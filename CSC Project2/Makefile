#Author: Kuan-Ju Chen
#Date: 2021/04/12
#ID: 0711529
#Describe: Prework for the mitm_attack
THE_MAKEFILE_FILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
THE_BASE_DIR_PATH := $(abspath $(dir $(THE_MAKEFILE_FILE_PATH)))
THE_BIN_DIR_PATH := $(THE_BASE_DIR_PATH)/bin

install:
	sh $(THE_BIN_DIR_PATH)/ip_forward.sh
	sh $(THE_BIN_DIR_PATH)/generate_ca.sh
	sh $(THE_BIN_DIR_PATH)/run_sslsplit.sh
	sh $(THE_BIN_DIR_PATH)/file_mode.sh
	
ip_forward:
	sh $(THE_BIN_DIR_PATH)/ip_forward.sh
	
generate_ca:
	sh $(THE_BIN_DIR_PATH)/generate_ca.sh
	
run_sslsplit:
	sh $(THE_BIN_DIR_PATH)/run_sslsplit.sh
	
file_mode:
	sh $(THE_BIN_DIR_PATH)/file_mode.sh