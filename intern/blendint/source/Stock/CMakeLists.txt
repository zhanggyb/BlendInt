# CMake file for BlendInt project
#

file(GLOB BlendIntStock_SRC "*.cpp")
file(GLOB BlendIntStockIntern_SRC "intern/*.cpp")

add_library (BlendIntStock ${BlendIntStock_SRC} ${BlendIntStockIntern_SRC})

target_link_libraries(BlendIntStock BlendIntCore BlendIntOpenGL BlendIntGui ${LIBS})

