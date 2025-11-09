file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

# 헤더 파일들이 위치한 디렉토리를 저장할 리스트
set(HEADER_DIRS "")

# 파일 목록을 순회하면서 디렉토리만 추출
foreach(header_file ${HEADER_FILES})
    get_filename_component(header_dir ${header_file} DIRECTORY)
    list(APPEND HEADER_DIRS ${header_dir})
endforeach()

# 중복된 디렉토리 제거
list(REMOVE_DUPLICATES HEADER_DIRS)

# 디렉토리들을 include_directories에 추가
include_directories(${HEADER_DIRS})
include_directories(/usr/include)
