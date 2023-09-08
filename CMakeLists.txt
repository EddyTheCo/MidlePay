cmake_minimum_required(VERSION 3.16)

include(local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(ccommon)
version_from_git(
	LOG       OFF
	TIMESTAMP "%Y%m%d%H%M%S"
	)
project(MidlePayQml VERSION ${VERSION} DESCRIPTION "MiddlePay QML module" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

FetchContent_Declare(
	qrCode
	GIT_REPOSITORY https://github.com/EddyTheCo/qrCode.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(qrCode)


FetchContent_Declare(
        qslip10
        GIT_REPOSITORY https://github.com/EddyTheCo/Qslip10.git
        GIT_TAG v0.0.1
        FIND_PACKAGE_ARGS 0 CONFIG
        )
FetchContent_MakeAvailable(qslip10)

FetchContent_Declare(
        qaddr_bundle
        GIT_REPOSITORY https://github.com/EddyTheCo/QAddrBundle.git
        GIT_TAG v0.2.2
        FIND_PACKAGE_ARGS 0 CONFIG
        )
FetchContent_MakeAvailable(qaddr_bundle)


FetchContent_Declare(
        qclient
        GIT_REPOSITORY https://github.com/EddyTheCo/Qclient-IOTA.git
        GIT_TAG v0.2.2
        FIND_PACKAGE_ARGS 0 CONFIG
        )
FetchContent_MakeAvailable(qclient)

FetchContent_Declare(
        qclientMQTT
        GIT_REPOSITORY https://github.com/EddyTheCo/QclientMqtt-IOTA.git
        GIT_TAG v0.2.2
        FIND_PACKAGE_ARGS 0 CONFIG
        )
FetchContent_MakeAvailable(qclientMQTT)

FetchContent_Declare(
	MyDesigns
	GIT_REPOSITORY https://github.com/EddyTheCo/MyDesigns.git
        GIT_TAG v0.2.2
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(MyDesigns)

find_package(Qt6 COMPONENTS Core Gui Quick)

if (Qt6_FOUND)
	qt_standard_project_setup()
	qt6_add_qml_module(midlePay
		URI  midlePay
		VERSION 1.0
		SOURCES
		src/midlePay.cpp include/midlePay.hpp
		QML_FILES
                qml/MidlePayPop.qml
		RESOURCE_PREFIX
		"/esterVtech.com/imports"
		OUTPUT_TARGETS out_targets_var
		OUTPUT_DIRECTORY
		${CMAKE_CURRENT_BINARY_DIR}/midlePay
		)

	target_link_libraries(midlePay PUBLIC Qt6::Gui Qt6::Quick)

	target_compile_definitions(midlePay PRIVATE WINDOWS_ACCOU)
	target_include_directories(midlePay PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/midlePay>")
        target_link_libraries(midlePay PUBLIC  QtQrGen qslip10 qaddr_bundle qclient qclientMQTT)


	install(TARGETS midlePay ${out_targets_var}
		EXPORT midlePay-config
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/midlePay
		)

	install(EXPORT midlePay-config
		FILE midlePayTargets.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/midlePay
		)
	include(CMakePackageConfigHelpers)
	configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/midlePayConfig.cmake"
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/midlePay
		)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/midlePayConfigVersion.cmake"
		VERSION ${VERSION}
		COMPATIBILITY SameMajorVersion
		)
	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/midlePayConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/midlePayConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/midlePay
		)
	export(EXPORT midlePay-config
		FILE "${CMAKE_CURRENT_BINARY_DIR}/midlePay-config.cmake"
		)
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		set(CPACK_PACKAGE_CONTACT "estervtech")
		include(CTest)
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
		if(USE_THREADS)
			set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-MThread")
		endif(USE_THREADS)
		include(CPack)
	endif()

endif(Qt6_FOUND)

if(BUILD_DOCS)
        get_target_property(build_docs cmake_build_docs SOURCES)
        include(${build_docs})
endif()


list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}/)
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "" FORCE)
