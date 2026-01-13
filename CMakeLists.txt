# C:\Users\marcb\.vcpkg-clion\vcpkg\triplets\community\x64-mingw-dynamic.cmake
# C:\Users\marcb\.vcpkg-clion\vcpkg\packages\protobuf_x64-mingw-dynamic\bin\{libprotoc.dll, libprotobuf.dll, libprotobuf-lite.dll}
# C:\Users\marcb\.vcpkg-clion\vcpkg\installed\x64-mingw-dynamic\bin\{libprotobuf.dll, libprotobuf-lite.dll, libabseil_dll.dll, libprotoc.dll}

cmake_minimum_required(VERSION 4.1)
project(TOTP_manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if (WIN32)
    set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
    set(VCPKG_INSTALLED_DIR
            "$ENV{USERPROFILE}/.vcpkg-clion/vcpkg/installed/${VCPKG_TARGET_TRIPLET}")
    list(PREPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")
endif()

find_package(Qt6 REQUIRED COMPONENTS
        Core Gui Widgets Sql
)
find_package(protobuf CONFIG REQUIRED)

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
if (WIN32)
    add_custom_command(TARGET TOTP_manager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VCPKG_INSTALLED_DIR}/bin/libprotobuf.dll"
            "$<TARGET_FILE_DIR:TOTP_manager>"
            )
    add_custom_command(TARGET TOTP_manager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VCPKG_INSTALLED_DIR}/bin/libprotobuf-lite.dll"
            "$<TARGET_FILE_DIR:TOTP_manager>"
            )
    add_custom_command(TARGET TOTP_manager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VCPKG_INSTALLED_DIR}/bin/libprotoc.dll"
            "$<TARGET_FILE_DIR:TOTP_manager>"
            )
    add_custom_command(TARGET TOTP_manager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VCPKG_INSTALLED_DIR}/bin/libabseil_dll.dll"
            "$<TARGET_FILE_DIR:TOTP_manager>"
            )
endif()