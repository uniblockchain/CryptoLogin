IF(NOT CRYPTOLOGIN_CMAKE)
SET(CRYPTOLOGIN_CMAKE 1)

INCLUDE(${GLOBAL_DIR}/Datastructures/CMakeLists.txt)
INCLUDE(${GLOBAL_DIR}/UnitTest/CMakeLists.txt)
INCLUDE(${GLOBAL_DIR}/CryptoGateway/CMakeLists.txt)
INCLUDE(${GLOBAL_DIR}/osMechanics/CMakeLists.txt)
INCLUDE(${GLOBAL_DIR}/glGraphics/CMakeLists.txt)

MESSAGE(STATUS )
MESSAGE(STATUS ========= CryptoLogin ==========)
MESSAGE(STATUS )

SET(CUR_SRC ${GLOBAL_DIR}/CryptoLogin)
INCLUDE_DIRECTORIES(${GLOBAL_DIR})

SET( SRC_H
	${CUR_SRC}/cryptoLogin.h
	${CUR_SRC}/loginMain.h
	${CUR_SRC}/cryptoLoginLog.h
	${CUR_SRC}/loginMetaData.h
	${CUR_SRC}/createUserForm.h
	${CUR_SRC}/listUsersForm.h
	${CUR_SRC}/keyGenerationPopUp.h
	${CUR_SRC}/manageKeysForm.h
)

SET( SRC_CPP
	${CUR_SRC}/loginMain.cpp
	${CUR_SRC}/loginMetaData.cpp
	${CUR_SRC}/createUserForm.cpp
	${CUR_SRC}/listUsersForm.cpp
	${CUR_SRC}/keyGenerationPopUp.cpp
	${CUR_SRC}/manageKeysForm.cpp
)

ADD_LIBRARY( CryptoLogin STATIC
	${SRC_CPP}
	${SRC_H}
)
TARGET_LINK_LIBRARIES( CryptoLogin
	${OS_LIBS}
	${OS_GL_LIBS}
	${EXE_LIBS}
)
SET(EXE_LIBS ${EXE_LIBS} CryptoLogin)

ENDIF(NOT CRYPTOLOGIN_CMAKE)