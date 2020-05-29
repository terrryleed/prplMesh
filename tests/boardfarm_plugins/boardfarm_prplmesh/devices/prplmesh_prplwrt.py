###############################################################
# SPDX-License-Identifier: BSD-2-Clause-Patent
# SPDX-FileCopyrightText: 2020 the prplMesh contributors (see AUTHORS.md)
# This code is subject to the terms of the BSD+Patent license.
# See LICENSE file for more details.
###############################################################

import boardfarm
import json
import os
import subprocess
import sys
import pexpect

from boardfarm.devices import openwrt_router, connection_decider
from boardfarm.lib import common
from boardfarm.lib.bft_pexpect_helper import bft_pexpect_helper
from environment import ALEntityPrplWrt, _get_bridge_interface
from .prplmesh_base import PrplMeshBase
from sniffer import Sniffer
from ipaddress import IPv4Network


class PrplMeshPrplWRT(openwrt_router.OpenWrtRouter, PrplMeshBase):
    """prplWRT burned device with prplMesh installed."""

    model = ("prplWRT")
    prompt = ['root\\@OpenWrt:/#', '/#', '@OpenWrt:/#']
    wan_iface = "eth1"
    uboot_eth = "eth0_1"
#    linesep = "\x0D"
    agent_entity = None
    controller_entity = None

    def __init__(self, *args, **kwargs):
        """Initialize device."""
        self.args = args
        self.kwargs = kwargs
        config = kwargs.get("config", kwargs)

        self.consoles = [self]
        self.linesep = "\r"
#        self.delaybeforesend = None
#        self.logfile_read = sys.stdout
#        self.logfile = None

        self.unique_id = os.getenv("SUDO_USER", os.getenv("USER", ""))
        self.docker_network = config.get("docker_network",
                                         "prplMesh-net-{}".format(self.unique_id))
        self.role = config.get("role", "agent")
        self.connection_type = config.get("connection_type")
        self.conn_cmd = config.get("conn_cmd")
        self.connected_iface = config.get("device_iface")
        self.name = "-".join((config.get("name", "prplmesh_docker"), self.unique_id))

        self.connection = connection_decider.connection(device=self,
                                                        conn_type=kwargs['connection_type'],
                                                        **kwargs)
        self.connection.connect()

        self.wan_network = self.get_docker_subnet(self.docker_network)
        self.set_iface_to_bridge(self, self.wan_iface)
        self.set_iface_ip(self, "br-lan", self.wan_network)
        self.wired_sniffer = Sniffer(_get_bridge_interface(self.docker_network),
                                     boardfarm.config.output_dir)
        if self.role == "controller":
            self.controller_entity = ALEntityPrplWrt(self.name, self, is_controller=True)
        else:
            self.agent_entity = ALEntityPrplWrt(self.name, self, is_controller=False)
            self.prplMesh_start_agent(self)

    def _prplMesh_exec(self, device: None, mode: str):
        """Send line to prplmesh initd script."""
        device.sendline("/etc/init.d/prplmesh {}".format(mode))

    def check_status(self) -> bool:
        """Check status of device, return bool to indicate state.

        It is used by boardfarm to indicate that spawned device instance is ready for test
        and also after test - to insure that device still operational.
        """
        return True

    def prplMesh_check_state(self) -> bool:
        self.sendline("/etc/init.d/prplmesh status")
        match = self.expect(
                ["operational", "FAIL", pexpect.EOF, pexpect.TIMEOUT],
                timeout=10)
        if match == 0:
            return True
        else:
            return False

    def isalive(self):
        """Check if device is alive.

        States that device is operational and its consoles are accessible.
        """
        return self.check_status()

    def touch(self):
        """Keep consoles alive.

        Purpose is to keep consoles active, so they don't disconnect for long running activities.
        """
        pass

    def get_docker_subnet(self, docker_network: str) -> IPv4Network:
        """Get subnet used by docker network."""
        docker_network_inspect_cmd = ('docker', 'network', 'inspect', docker_network)
        inspect_raw = subprocess.run(docker_network_inspect_cmd, stdout=subprocess.PIPE)
        if inspect_raw.returncode != 0:
            # Assume network doesn't exist yet. Create it.
            # Raise an exception if it fails (check=True).
            subprocess.run(('docker', 'network', 'create', docker_network), check=True,
                           stdout=subprocess.DEVNULL)
            # Inspect again, now raise if it fails (check=True).
            inspect_raw= subprocess.run(docker_network_inspect_cmd, check=True,
                                            stdout=subprocess.PIPE)

        inspect_json = json.loads(inspect_raw.stdout)
        return IPv4Network(inspect_json[0]["IPAM"]["Config"][0]["Subnet"])

    def set_iface_to_bridge(self, device: None, iface: str) -> bool:
        """Add specified interface to the specified bridge."""
        ip_command = ("ip link set {} master br-lan".format(iface))
        device.sendline(ip_command)
        match = device.expect([":/#", pexpect.EOF, pexpect.TIMEOUT], timeout=10)
        if match == 1:
            return True
        else:
            return False
        return True

    def set_iface_ip(self, device: None, iface: str, ip: IPv4Network) -> bool:
        """Set interface IPv4 address."""
        device.sendline("ip a add {}/{} dev {}".format(ip[+245], ip.prefixlen, iface))
        match = device.expect([":/#", pexpect.EOF, pexpect.TIMEOUT], timeout=10)
        if match == 1:
            return True
        else:
            return False

    def prplMesh_start_agent(self, device: None) -> bool:
        """Start prplMesh in certification_mode agent. Return true if done."""
#        self._prplMesh_exec(device, "certification_mode agent")
#        match = device.expect_exact(["wlan0.3: start the interface",
#                              pexpect.EOF, pexpect.TIMEOUT], timeout=90)
        device.sendline("/etc/init.d/prplmesh certification_mode agent")
        device.expect("CAC timer expired", timeout=120)
        match = device.expect("device br-lan entered promiscuous mode", timeout=40)
        if match == 0:
            return True
        else:
            return False
