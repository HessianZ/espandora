file(GLOB_RECURSE ESPANDORA_SOURCES ./*.c)

idf_component_register(
    SRC_DIRS
        "."
        "app"
        "gui"
        "http"
        "gui/font"
        "gui/image"

    INCLUDE_DIRS
        "."
        "app"
        "gui"
        "http"
)

add_definitions(-w)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")

target_compile_definitions(${COMPONENT_TARGET} PRIVATE "-D RMAKER_DEMO_PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")

set_source_files_properties(
        ${ESPANDORA_SOURCES}
        PROPERTIES COMPILE_OPTIONS
        -DLV_LVGL_H_INCLUDE_SIMPLE)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)

set(voice_data_image ${PROJECT_DIR}/esp_tts_voice_data_xiaoxin_small.dat)
add_custom_target(voice_data ALL DEPENDS ${voice_data_image})
add_dependencies(flash voice_data)

partition_table_get_partition_info(size "--partition-name voice_data" "size")
partition_table_get_partition_info(offset "--partition-name voice_data" "offset")

if("${size}" AND "${offset}")
    esptool_py_flash_to_partition(flash "voice_data" "${voice_data_image}")
else()
    set(message "Failed to find model in partition table file"
            "Please add a line(Name=voice_data, Type=data, Size=3890K) to the partition file.")
endif()