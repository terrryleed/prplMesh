message("${BoldWhite}Preparing ${BoldGreen}common${BoldWhite} for the ${BoldGreen}${TARGET_PLATFORM}${BoldWhite} platform${ColourReset}")

# Default values
if (NOT DEFINED BEEROCKS_PLATFORM_INIT)
    set(BEEROCKS_PLATFORM_INIT "false")
endif()

# Modules
add_subdirectory(beerocks)
