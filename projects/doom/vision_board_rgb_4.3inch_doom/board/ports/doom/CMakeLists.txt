file(GLOB DoomSources *.c)
file(GLOB PlatformSources platform/*.c)

idf_component_register(SRCS ${DoomSources} ${PlatformSources}
                    INCLUDE_DIRS "include" "platform")

set(NEW_FLAGS
	-Wno-char-subscripts
	-Wno-unused-value
	-Wno-parentheses
	-Wno-int-to-pointer-cas
	-Wno-pointer-sign
	-Wno-unused-but-set-parameter
	-Wno-maybe-uninitialized
	-Wno-unused-variable
	-Wno-unused-const-variable
	-Wno-format-truncation
)

target_compile_options(${COMPONENT_LIB} PRIVATE ${NEW_FLAGS})
