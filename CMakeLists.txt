# C:\Users\marcb\.vcpkg-clion\vcpkg\triplets\community\x64-mingw-dynamic.cmake
# C:\Users\marcb\.vcpkg-clion\vcpkg\packages\protobuf_x64-mingw-dynamic\bin\{libprotoc.dll, libprotobuf.dll, libprotobuf-lite.dll}
# C:\Users\marcb\.vcpkg-clion\vcpkg\installed\x64-mingw-dynamic\bin\{libprotobuf.dll, libprotobuf-lite.dll, libabseil_dll.dll, libprotoc.dll}
cmake_minimum_required(VERSION 4.1)
project(TOTP_manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -------------------------------
# vcpkg MinGW dynamic fix
# -------------------------------
if (WIN32 AND DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_INSTALLED_DIR
            "$ENV{USERPROFILE}/.vcpkg-clion/vcpkg/installed/${VCPKG_TARGET_TRIPLET}")
    list(PREPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")
endif()

# -------------------------------
# Qt
# -------------------------------
find_package(Qt6 REQUIRED COMPONENTS
        Core Gui Widgets Sql
)

# -------------------------------
# Protobuf (modern CONFIG mode)
# -------------------------------
find_package(protobuf CONFIG REQUIRED)

# -------------------------------
# Protobuf code generation
# -------------------------------
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proto/otpauth-migration.proto")
set(GENERATED_PROTO_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${GENERATED_PROTO_DIR}")

add_custom_command(
        OUTPUT
        "${GENERATED_PROTO_DIR}/otpauth-migration.pb.cc"
        "${GENERATED_PROTO_DIR}/otpauth-migration.pb.h"
        COMMAND protobuf::protoc
        ARGS
        --cpp_out="${GENERATED_PROTO_DIR}"
        -I "${CMAKE_CURRENT_SOURCE_DIR}/proto"
        "${PROTO_FILE}"
        DEPENDS "${PROTO_FILE}"
        COMMENT "Generating protobuf sources"
)

set(GENERATED_PROTO_SRCS
        "${GENERATED_PROTO_DIR}/otpauth-migration.pb.cc"
)
set(GENERATED_PROTO_HDRS
        "${GENERATED_PROTO_DIR}/otpauth-migration.pb.h"
)

# -------------------------------
# Sources
# -------------------------------
add_executable(TOTP_manager
        src/main.cpp
        src/controllers/AccountController.cpp
        src/models/Account.cpp
        src/models/Storage.cpp
        src/models/TotpGenerator.cpp
        src/gui/MainWindow.cpp
        src/gui/AddAccountDialog.cpp
        src/gui/SelectAccountsDialog.cpp
        src/gui/widgets/TotpCard.cpp
        src/utils/Clipboard.cpp
        src/utils/OtpauthMigration.cpp
        ${GENERATED_PROTO_SRCS}
        ${GENERATED_PROTO_HDRS}
)

# -------------------------------
# Includes
# -------------------------------
target_include_directories(TOTP_manager PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GENERATED_PROTO_DIR}
)

# -------------------------------
# Link libraries
# -------------------------------
target_link_libraries(TOTP_manager PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
        protobuf::libprotobuf
        protobuf::libprotoc
)

# -------------------------------
# Windows Qt runtime deployment
# -------------------------------
# -------------------------------
# Windows runtime DLL deployment
# -------------------------------
if (WIN32)
    # Qt DLLs
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

    # vcpkg DLLs
    set(VCPKG_BIN_DIR "$ENV{USERPROFILE}/.vcpkg-clion/vcpkg/installed/${VCPKG_TARGET_TRIPLET}/bin")

    # MinGW runtime DLLs
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

    # Tous les DLLs à copier
    file(GLOB RUNTIME_DLLS
            "${QT_BIN_DIR}/*.dll"
            "${VCPKG_BIN_DIR}/*.dll"
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
    )

    # Copier toutes les DLLs après build
    add_custom_command(TARGET TOTP_manager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RUNTIME_DLLS}
            "$<TARGET_FILE_DIR:TOTP_manager>"
    )
endif()