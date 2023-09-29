##########################################################################
# Makefile                                                               #
# _________                        ___________                           #
# \_   ___ \  ___________   ____   \_   _____/__________  _____   ______ #
# /    \  \/ /  _ \_  __ \_/ __ \   |    __)/  _ \_  __ \/     \ /  ___/ #
# \     \___(  <_> )  | \/\  ___/   |     \(  <_> )  | \/  Y Y  \\___ \  #
#  \______  /\____/|__|    \___  >  \___  / \____/|__|  |__|_|  /____  > #
#         \/                   \/       \/                    \/     \/  #
##########################################################################
cmake_minimum_required (VERSION 3.8.2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Solution: CoreForms
project (CoreForms
	#DESCRIPTION "CORE library providing an intuitive windowing abstraction"
	LANGUAGES CXX)
message("solution: ${PROJECT_SOURCE_DIR}")

# Define base folder
set(BASE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/")
set(OUTPUT_FOLDER "${BASE_FOLDER}build")

# Folders: Project
set(PROJECT_EXTRA_FOLDER    "${BASE_FOLDER}extra/")
set(PROJECT_INCLUDE_FOLDER  "${BASE_FOLDER}include/")
set(PROJECT_SAMPLE_FOLDER   "${BASE_FOLDER}sample/")
set(PROJECT_SOURCE_FOLDER   "${BASE_FOLDER}src/")
set(PROJECT_RESOURCE_FOLDER "${BASE_FOLDER}res/")

# Folders: Test code
set(PROJECT_TEST_FOLDER     "${BASE_FOLDER}test/")
set(PROJECT_MOCK_FOLDER     "${BASE_FOLDER}mock/")

# Folders: Google.Mock
set(GMOCK_FOLDER         "${BASE_FOLDER}3rdparty/googlemock/")
set(GMOCK_INCLUDE_FOLDER "${GMOCK_FOLDER}include/")
set(GMOCK_SOURCE_FOLDER  "${GMOCK_FOLDER}src/")

# Folders: Google.Test
set(GTEST_FOLDER         "${BASE_FOLDER}3rdparty/googletest/")
set(GTEST_INCLUDE_FOLDER "${GTEST_FOLDER}include/")
set(GTEST_SOURCE_FOLDER  "${GTEST_FOLDER}src/")

# Set Guideline Support Library contract violation behaviour
set(GSL_ASSERTION_BEHAVIOUR "GSL_THROW_ON_CONTRACT_VIOLATION")

# Set compiler options used by all modules
# /std:c++latest	: Preview ISO C++ 23 Standard
# /std:c17			: ISO C 17 Standard
# /permissive		: Support non-standard Microsoft COM attributes
# /Zc:preprocessor	: ISO C++20 Preprocessor
# /ZI				: PDBs for edit & continue
# /EHa				: Enable SEH and C++ exceptions
# /W4				: Warning level 4
# /wd4100			: Disable W4100: Unreferenced formal parameter
# /wd4251			: Disable W4251: Classes containing fields of STL type (CRT compatibility warning)
# /wd4275			: Disable W4275: Classes containing bases of STL type (CRT compatibility warning)
# /wd4467           : Disable C4467: Usage of ATL attributes is deprecated
# /wd4684           : Disable C4684: 'no_injected_text': WARNING!! attribute may cause invalid code generation: use with caution
# /wd4543           : Disable C4543: Injected text suppressed by attribute 'no_injected_text'
# /wd4584           : Disable C4584: Base-class 'IUnknown' is already a base-class of 'I...'
set(GLOBAL_COMPILER_OPTIONS "/std:c++latest /std:c17 /permissive /Zc:preprocessor /ZI /EHa /W4 /wd4100 /wd4251 /wd4275 /wd4467 /wd4684 /wd4543 /wd4584")

# Set linker options used by all modules
set(GLOBAL_LINKER_OPTIONS "${GLOBAL_LINKER_OPTIONS} /SAFESEH:NO")

# Set global preprocessor directives:
# _ATL_ATTRIBUTES                           : Enable non-standard Microsoft COM attributes
# BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 	: Disable MSVC-2017 forward-compatibility warning
# _SILENCE_CXX20_CODECVT_FACETS_DEPRECAT... : Disable C++20 codecvt deprecation warning
# _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 	: Disable C++17 deprecation warnings
# _CRT_SECURE_NO_WARNINGS 					: Disable warnings about documented behaviour of CRT functions
# _SCL_SECURE_NO_WARNINGS					: Disable warnings about documented behaviour of C++ library functions
# GSL_ASSERTION_BEHAVIOUR 					: Globally set the contract-violation behaviour of GSL
# _UNICODE 									: Build everything in the UNICODE character set
add_definitions(-D_ATL_ATTRIBUTES
	-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 
	-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
	-D_SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING
	-D_CRT_SECURE_NO_WARNINGS
	-D_SCL_SECURE_NO_WARNINGS
	-D${GSL_ASSERTION_BEHAVIOUR}
	-D_UNICODE
	-DNOMINMAX)

# Set global include folders: 
# $ENV{BOOST_ROOT}	:	Boost headers
# $ENV{GSL_ROOT}	:	Guideline Support Library (GSL) headers
include_directories($ENV{BOOST_ROOT} 
	$ENV{GSL_ROOT}
	"${BASE_FOLDER}../Core-Platform/include"
	"${BASE_FOLDER}../Core-Com/include"
)

# Set global library folders:
# $ENV{BOOST_ROOT}/stage/lib	: Boost libraries
# ${OUTPUT_FOLDER}				: Solution output folder
link_directories($ENV{BOOST_ROOT}/stage/lib ${OUTPUT_FOLDER})
	
# Remove the 'RelWithDebInfo' and 'MinSizeRel' configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )
#set(CMAKE_CONFIGURATION_TYPES Debug CACHE TYPE INTERNAL FORCE )

# Source files
file(GLOB_RECURSE PROJECT_SOURCE_FILES
    "${PROJECT_SOURCE_FOLDER}*.h"
    "${PROJECT_SOURCE_FOLDER}*.cpp"
)
	
# [OPTIMIZED] Source files to exclude by marking as header-only
file(GLOB_RECURSE PROJECT_SOURCE_IMPL_FILES
    "${PROJECT_SOURCE_FOLDER}*.cpp"
    )
set_source_files_properties(${PROJECT_SOURCE_IMPL_FILES} PROPERTIES 
    HEADER_FILE_ONLY ON
)

# Optimized-build files
file(GLOB_RECURSE PROJECT_SOURCE_CC_FILES
    "${PROJECT_SOURCE_FOLDER}*.cc"
)
	
# Exported header files
file(GLOB_RECURSE PROJECT_INCLUDE_FILES
    "${PROJECT_INCLUDE_FOLDER}*.h"
)

# Resource files
file(GLOB_RECURSE PROJECT_RESOURCE_FILES
    "${PROJECT_RESOURCE_FOLDER}*.*"
)

# Define macro controlling dllexport/import specifier
set(PROJECT_EXPORT_MACRO "CORE_FORMS_EXPORT")
set(PROJECT_IMPORT_MACRO "CORE_FORMS_IMPORT")
set(PROJECT_STATIC_MACRO "CORE_FORMS_STATIC")

##############################################################################
#         _________ __          __  .__         .____    ._____.             #
#        /   _____//  |______ _/  |_|__| ____   |    |   |__\_ |__           #
#        \_____  \\   __\__  \\   __\  |/ ___\  |    |   |  || __ \          #
#        /        \|  |  / __ \|  | |  \  \___  |    |___|  || \_\ \         #
#       /_______  /|__| (____  /__| |__|\___  > |_______ \__||___  /         #
#               \/           \/             \/          \/       \/          #
##############################################################################
message("StaticLib: Generating project(s) from ${PROJECT_SOURCE_FOLDER} with ${PROJECT_INCLUDE_FOLDER}")

# Project: libCoreForms
add_library(libCoreForms STATIC 
	"${PROJECT_EXTRA_FOLDER}Core-Forms.natvis"
	${PROJECT_SOURCE_FILES} 
	${PROJECT_SOURCE_CC_FILES} 
	${PROJECT_INCLUDE_FILES} 
)
  
# Include folders
target_include_directories(libCoreForms BEFORE
    PRIVATE
	${PROJECT_SOURCE_FOLDER}
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}
)
	
# Set linker dependencies 
target_link_libraries(libCoreForms
	libCoreCom
	#libCorePlatform
	UxTheme
	Msimg32
)

# Set compile flags
set_target_properties(libCoreForms PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
)

# Set export macros to 'static library' (same for all dependencies)
target_compile_definitions(libCoreForms
	PUBLIC ${PROJECT_STATIC_MACRO}
	PUBLIC CORE_COM_STATIC
	PUBLIC CORE_PLATFORM_STATIC
)

# Link CRT statically
target_compile_options(libCoreForms
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
#add_dependencies(libCoreForms
#	libCoreCOM
#	libCorePlatform
#)

#############################################################################
#  ________                              .__          .____    ._____.      #
#  \______ \ ___.__. ____ _____    _____ |__| ____    |    |   |__\_ |__    #
#   |    |  <   |  |/    \\__  \  /     \|  |/ ___\   |    |   |  || __ \   #
#   |    `   \___  |   |  \/ __ \|  Y Y  \  \  \___   |    |___|  || \_\ \  #
#  /_______  / ____|___|  (____  /__|_|  /__|\___  >  |_______ \__||___  /  #
#          \/\/         \/     \/      \/        \/           \/       \/   #
#############################################################################
message("DynamicLib: Generating project(s) from ${PROJECT_SOURCE_FOLDER} with ${PROJECT_INCLUDE_FOLDER}")

# Project: dllCoreForms
add_library(dllCoreForms SHARED 
	"${PROJECT_EXTRA_FOLDER}DllMain.cpp"
	"${PROJECT_EXTRA_FOLDER}Core-Forms.natvis"
	${PROJECT_SOURCE_FILES} 
	${PROJECT_SOURCE_CC_FILES} 
	${PROJECT_INCLUDE_FILES} 
)
  
# Include folders
target_include_directories(dllCoreForms BEFORE
    PRIVATE
	${PROJECT_SOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}
)

# Differentiate the filename of the import library
set_target_properties(dllCoreForms PROPERTIES
					  OUTPUT_NAME "CoreForms"
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
)

# Set linker dependencies
target_link_libraries(dllCoreForms
	#dllCorePlatform
	#dllCoreCom
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/CorePlatform.lib"
	"${BASE_FOLDER}../Core-Com/build/$(Configuration)/CoreCom.lib"
)

# Set export macros to 'export DLL' and set dependencies to 'import DLL'
target_compile_definitions(dllCoreForms
	PUBLIC ${PROJECT_EXPORT_MACRO}
	PUBLIC CORE_COM_IMPORT
	PUBLIC CORE_PLATFORM_IMPORT
)

# Link CRT dynamically
target_compile_options(dllCoreForms
	PRIVATE $<$<CONFIG:Release>:/MD>
	PRIVATE $<$<CONFIG:Debug>:/MDd>
)

# Set project dependencies
#add_dependencies(dllCoreForms
#	dllCoreCom
#	dllCorePlatform
#)

#########################################################################
#  __________               .__         ___________                     #
#  \______   \_____    _____|__| ____   \_   _____/__________  _____    #
#   |    |  _/\__  \  /  ___/  |/ ___\   |    __)/  _ \_  __ \/     \   #
#   |    |   \ / __ \_\___ \|  \  \___   |     \(  <_> )  | \/  Y Y  \  #
#   |______  /(____  /____  >__|\___  >  \___  / \____/|__|  |__|_|  /  #
#          \/      \/     \/        \/       \/                    \/   #
#########################################################################

# Project: appBasicForm
add_executable(appBasicForm
	"${PROJECT_SAMPLE_FOLDER}appBasicForm.cpp"
	${PROJECT_RESOURCE_FILES}
    )

# Include folders
target_include_directories(appBasicForm BEFORE
    PRIVATE
	${PROJECT_SAMPLE_FOLDER} 
	${PROJECT_RESOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}            
	)
	
# Set linker dependencies
target_link_libraries(appBasicForm
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/libCorePlatform.lib"
	"${BASE_FOLDER}../Core-Com/build/$(Configuration)/libCoreCom.lib"
	"${BASE_FOLDER}../Core-Forms/build/$(Configuration)/libCoreForms.lib"
)

# Set compiler & linker flags
set_target_properties(appBasicForm PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:CONSOLE"
					  OUTPUT_NAME "appBasicForm"
					  FOLDER "Samples"
)

# Set export macros for static-library for all dependencies
target_compile_definitions(appBasicForm
	PRIVATE CORE_PLATFORM_STATIC CORE_COM_STATIC CORE_FORMS_STATIC
)

# Link CRT statically
target_compile_options(appBasicForm
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
add_dependencies(appBasicForm
	libCoreForms
)

##############################################################
#  _________                __                .__          	 #
#  \_   ___ \  ____   _____/  |________  ____ |  |   ______	 #
#  /    \  \/ /  _ \ /    \   __\_  __ \/  _ \|  |  /  ___/	 #
#  \     \___(  <_> )   |  \  |  |  | \(  <_> )  |__\___ \ 	 #
#   \______  /\____/|___|  /__|  |__|   \____/|____/____  >	 #
#          \/            \/                             \/ 	 #
##############################################################

# Project: appControls
add_executable(appControls
	"${PROJECT_SAMPLE_FOLDER}appControls.cpp"
	${PROJECT_RESOURCE_FILES}
    )

# Include folders
target_include_directories(appControls BEFORE
    PRIVATE
	${PROJECT_SAMPLE_FOLDER} 
	${PROJECT_RESOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}            
	)
	
# Set linker dependencies
target_link_libraries(appControls
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/libCorePlatform.lib"
	"${BASE_FOLDER}../Core-Com/build/$(Configuration)/libCoreCom.lib"
	"${BASE_FOLDER}../Core-Forms/build/$(Configuration)/libCoreForms.lib"
)

# Set compiler & linker flags
set_target_properties(appControls PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:CONSOLE"
					  OUTPUT_NAME "appControls"
					  FOLDER "Samples"
)

# Set export macros for static-library for all dependencies
target_compile_definitions(appControls
	PRIVATE CORE_PLATFORM_STATIC CORE_COM_STATIC CORE_FORMS_STATIC
)

# Link CRT statically
target_compile_options(appControls
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
add_dependencies(appControls
	libCoreForms
)

############################################################################
# ________                ___________              __  .__                 #
# \______ \   _______  __ \__    ___/___   _______/  |_|__| ____    ____   #
#  |    |  \_/ __ \  \/ /   |    |_/ __ \ /  ___/\   __\  |/    \  / ___\  #
#  |    `   \  ___/\   /    |    |\  ___/ \___ \  |  | |  |   |  \/ /_/  > #
# /_______  /\___  >\_/     |____| \___  >____  > |__| |__|___|  /\___  /  #
#         \/     \/                    \/     \/               \//_____/   #
############################################################################

# Project: appDevTesting
add_executable(appDevTesting
	"${PROJECT_SAMPLE_FOLDER}appDevTesting.cpp"
	${PROJECT_RESOURCE_FILES}
    )

# Include folders
target_include_directories(appDevTesting BEFORE
    PRIVATE
	${PROJECT_SAMPLE_FOLDER} 
	${PROJECT_RESOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}            
	)
	
# Set linker dependencies
target_link_libraries(appDevTesting
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/libCorePlatform.lib"
	"${BASE_FOLDER}../Core-Com/build/$(Configuration)/libCoreCom.lib"
	"${BASE_FOLDER}../Core-Forms/build/$(Configuration)/libCoreForms.lib"
)

# Set compiler & linker flags
set_target_properties(appDevTesting PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:CONSOLE"
					  OUTPUT_NAME "appDevTesting"
					  FOLDER "Samples"
)

# Set export macros for static-library for all dependencies
target_compile_definitions(appDevTesting
	PRIVATE CORE_PLATFORM_STATIC CORE_COM_STATIC CORE_FORMS_STATIC
)

# Link CRT statically
target_compile_options(appDevTesting
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
add_dependencies(appDevTesting
	libCoreForms
)

###############################################################################
#   ________                     .__           ___________              __    #
#  /  _____/  ____   ____   ____ |  |   ____   \__    ___/___   _______/  |_  #
# /   \  ___ /  _ \ /  _ \ / ___\|  | _/ __ \    |    |_/ __ \ /  ___/\   __\ #
# \    \_\  (  <_> |  <_> ) /_/  >  |_\  ___/    |    |\  ___/ \___ \  |  |   #
#  \______  /\____/ \____/\___  /|____/\___  >   |____| \___  >____  > |__|   #
#         \/             /_____/           \/               \/     \/         #
###############################################################################
message("dllGoogleTest: Generating project(s) from ${GTEST_SOURCE_FOLDER} with ${GMOCK_SOURCE_FOLDER}")

# Project: dllGoogleTest
add_library(dllGoogleTest SHARED 
    "${GTEST_SOURCE_FOLDER}gtest-all.cc"
    "${GMOCK_SOURCE_FOLDER}gmock-all.cc"
    "${GTEST_SOURCE_FOLDER}gtest_main.cc"
)

# Include folders
target_include_directories(dllGoogleTest BEFORE
    PRIVATE
	${GTEST_FOLDER}
	${GMOCK_FOLDER}
    PUBLIC
    ${GTEST_INCLUDE_FOLDER}
	${GMOCK_INCLUDE_FOLDER}
)

# Set compiler & linker flags
set_target_properties(dllGoogleTest PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS}"
					  OUTPUT_NAME "GoogleTest"
					  FOLDER "Testing"
)

# Specify GTest should be build as a DLL
target_compile_definitions(dllGoogleTest
	PRIVATE GTEST_CREATE_SHARED_LIBRARY=1
)

# Link CRT dynamically
target_compile_options(dllGoogleTest
	PRIVATE $<$<CONFIG:Release>:/MD>
	PRIVATE $<$<CONFIG:Debug>:/MDd>
)
####################################################################
#  ____ ___      .__  __    ___________              __            #
# |    |   \____ |__|/  |_  \__    ___/___   _______/  |_  ______  #
# |    |   /    \|  \   __\   |    |_/ __ \ /  ___/\   __\/  ___/  #
# |    |  /   |  \  ||  |     |    |\  ___/ \___ \  |  |  \___ \   #
# |______/|___|  /__||__|     |____| \___  >____  > |__| /____  >  #
#              \/                        \/     \/            \/   #
####################################################################
message("exeFormsTests: Generating project(s) from ${PROJECT_SOURCE_FOLDER} with ${PROJECT_SOURCE_FOLDER}")

# Test files
file(GLOB_RECURSE PROJECT_UNIT_TEST_FILES
    "${PROJECT_TEST_FOLDER}*.h"
    "${PROJECT_TEST_FOLDER}*.cpp"
    )

# Project: exeFormsTests
add_executable(exeFormsTests
	${PROJECT_UNIT_TEST_FILES}
)

# Include folders
target_include_directories(exeFormsTests BEFORE
    PRIVATE
	${PROJECT_INCLUDE_FOLDER} 
	${PROJECT_SOURCE_FOLDER} 
	${PROJECT_TEST_FOLDER} 
)

# Consume GoogleTest as DLL
target_compile_definitions(exeFormsTests
	PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY=1
)

# Set compiler & linker flags
set_target_properties(exeFormsTests PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:CONSOLE"
					  FOLDER "Testing"
)

# Set linker dependencies
target_link_libraries(exeFormsTests
	dllGoogleTest
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/CorePlatform.lib" 
	"${BASE_FOLDER}../Core-COM/build/$(Configuration)/CoreCOM.lib" 
	dllCoreForms
)

# Set header declarations to dllimport
target_compile_definitions(exeFormsTests
	PRIVATE CORE_PLATFORM_IMPORT 
	        CORE_COM_IMPORT
	        ${PROJECT_IMPORT_MACRO}
)

# Link CRT dynamically
target_compile_options(exeFormsTests
	PRIVATE $<$<CONFIG:Release>:/MD>
	PRIVATE $<$<CONFIG:Debug>:/MDd>
)

# Set project dependencies
add_dependencies(exeFormsTests
	dllGoogleTest
	#dllCorePlatform
	#dllCoreCOM
	dllCoreForms
)
#################################################################################
#     ___________.__.__           .____                                __   
#     \_   _____/|__|  |   ____   |    |   _____  ___.__. ____  __ ___/  |_ 
#      |    __)  |  |  | _/ __ \  |    |   \__  \<   |  |/  _ \|  |  \   __\
#      |     \   |  |  |_\  ___/  |    |___ / __ \\___  (  <_> )  |  /|  |  
#      \___  /   |__|____/\___  > |_______ (____  / ____|\____/|____/ |__|  
#          \/                 \/          \/    \/\/                        
#################################################################################

# Extra files
file(GLOB_RECURSE PROJECT_EXTRA_FILES
    "${PROJECT_EXTRA_FOLDER}*.h"
    "${PROJECT_EXTRA_FOLDER}*.cpp"
)

# Sample files
file(GLOB_RECURSE PROJECT_SAMPLE_FILES
    "${PROJECT_SAMPLE_FOLDER}*.h"
    "${PROJECT_SAMPLE_FOLDER}*.cpp"
)

# Project file groups
source_group(TREE ${PROJECT_SOURCE_FOLDER}    PREFIX "Build Files"    FILES ${PROJECT_SOURCE_CC_FILES}})
source_group(TREE ${PROJECT_EXTRA_FOLDER}     PREFIX "Extra Files"    FILES ${PROJECT_EXTRA_FILES})
source_group(TREE ${PROJECT_INCLUDE_FOLDER}   PREFIX "Header Files"   FILES ${PROJECT_INCLUDE_FILES})
source_group(TREE ${PROJECT_SAMPLE_FOLDER}    PREFIX "Source Files"   FILES ${PROJECT_SAMPLE_FILES})
source_group(TREE ${PROJECT_SOURCE_FOLDER}    PREFIX "Source Files"   FILES ${PROJECT_SOURCE_FILES})
source_group(TREE ${PROJECT_RESOURCE_FOLDER}  PREFIX "Resource Files" FILES ${PROJECT_RESOURCE_FILES})

# unit-test file groups
#source_group(TREE ${PROJECT_TEST_FOLDER}   PREFIX "Build Files"  FILES ${PROJECT_UNIT_TEST_CC_FILES})
source_group(TREE ${PROJECT_TEST_FOLDER}   PREFIX "Test Files"   FILES ${PROJECT_UNIT_TEST_FILES})
source_group(TREE ${PROJECT_MOCK_FOLDER}   PREFIX "Mock Files"   FILES ${PROJECT_MOCK_FILES})
