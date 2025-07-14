#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/config/default/driver/usart/src/drv_usart.c ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1.c ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1_device.c ../src/config/default/osal/osal_freertos.c ../src/config/default/peripheral/clock/plib_clock.c ../src/config/default/peripheral/cmcc/plib_cmcc.c ../src/config/default/peripheral/dmac/plib_dmac.c ../src/config/default/peripheral/evsys/plib_evsys.c ../src/config/default/peripheral/nvic/plib_nvic.c ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/default/peripheral/port/plib_port.c ../src/config/default/peripheral/sercom/usart/plib_sercom4_usart.c ../src/config/default/peripheral/sercom/usart/plib_sercom5_usart.c ../src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c ../src/config/default/peripheral/tc/plib_tc0.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/system/cache/sys_cache.c ../src/config/default/system/command/src/sys_command.c ../src/config/default/system/console/src/sys_console_usb_cdc.c ../src/config/default/system/console/src/sys_console.c ../src/config/default/system/int/src/sys_int.c ../src/config/default/system/reset/sys_reset.c ../src/config/default/usb/src/usb_device_cdc.c ../src/config/default/usb/src/usb_device_cdc_acm.c ../src/config/default/usb/src/usb_device.c ../src/config/default/startup_xc32.c ../src/config/default/exceptions.c ../src/config/default/initialization.c ../src/config/default/interrupts.c ../src/config/default/libc_syscalls.c ../src/config/default/usb_device_init_data.c ../src/config/default/freertos_hooks.c ../src/config/default/tasks.c ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F/port.c ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c ../src/third_party/rtos/FreeRTOS/Source/timers.c ../src/third_party/rtos/FreeRTOS/Source/list.c ../src/third_party/rtos/FreeRTOS/Source/FreeRTOS_tasks.c ../src/third_party/rtos/FreeRTOS/Source/queue.c ../src/third_party/rtos/FreeRTOS/Source/croutine.c ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c ../src/third_party/rtos/FreeRTOS/Source/event_groups.c startup_samd51j19a.c system_samd51j19a.c ../src/main.c ../src/app.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/869808652/drv_usart.o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o ${OBJECTDIR}/_ext/1529399856/osal_freertos.o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ${OBJECTDIR}/_ext/1865161661/plib_dmac.o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ${OBJECTDIR}/_ext/1865521619/plib_port.o ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ${OBJECTDIR}/_ext/1376093119/sys_command.o ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o ${OBJECTDIR}/_ext/1832805299/sys_console.o ${OBJECTDIR}/_ext/1881668453/sys_int.o ${OBJECTDIR}/_ext/1000052432/sys_reset.o ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o ${OBJECTDIR}/_ext/308758920/usb_device.o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o ${OBJECTDIR}/_ext/1171490990/tasks.o ${OBJECTDIR}/_ext/246609638/port.o ${OBJECTDIR}/_ext/1665200909/heap_1.o ${OBJECTDIR}/_ext/404212886/timers.o ${OBJECTDIR}/_ext/404212886/list.o ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o ${OBJECTDIR}/_ext/404212886/queue.o ${OBJECTDIR}/_ext/404212886/croutine.o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ${OBJECTDIR}/_ext/404212886/event_groups.o ${OBJECTDIR}/startup_samd51j19a.o ${OBJECTDIR}/system_samd51j19a.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/app.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/869808652/drv_usart.o.d ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o.d ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o.d ${OBJECTDIR}/_ext/1529399856/osal_freertos.o.d ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d ${OBJECTDIR}/_ext/1865161661/plib_dmac.o.d ${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/1865521619/plib_port.o.d ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o.d ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o.d ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o.d ${OBJECTDIR}/_ext/829342655/plib_tc0.o.d ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d ${OBJECTDIR}/_ext/1014039709/sys_cache.o.d ${OBJECTDIR}/_ext/1376093119/sys_command.o.d ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o.d ${OBJECTDIR}/_ext/1832805299/sys_console.o.d ${OBJECTDIR}/_ext/1881668453/sys_int.o.d ${OBJECTDIR}/_ext/1000052432/sys_reset.o.d ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o.d ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o.d ${OBJECTDIR}/_ext/308758920/usb_device.o.d ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d ${OBJECTDIR}/_ext/1171490990/exceptions.o.d ${OBJECTDIR}/_ext/1171490990/initialization.o.d ${OBJECTDIR}/_ext/1171490990/interrupts.o.d ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o.d ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o.d ${OBJECTDIR}/_ext/1171490990/tasks.o.d ${OBJECTDIR}/_ext/246609638/port.o.d ${OBJECTDIR}/_ext/1665200909/heap_1.o.d ${OBJECTDIR}/_ext/404212886/timers.o.d ${OBJECTDIR}/_ext/404212886/list.o.d ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o.d ${OBJECTDIR}/_ext/404212886/queue.o.d ${OBJECTDIR}/_ext/404212886/croutine.o.d ${OBJECTDIR}/_ext/404212886/stream_buffer.o.d ${OBJECTDIR}/_ext/404212886/event_groups.o.d ${OBJECTDIR}/startup_samd51j19a.o.d ${OBJECTDIR}/system_samd51j19a.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/app.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/869808652/drv_usart.o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o ${OBJECTDIR}/_ext/1529399856/osal_freertos.o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ${OBJECTDIR}/_ext/1865161661/plib_dmac.o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ${OBJECTDIR}/_ext/1865521619/plib_port.o ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ${OBJECTDIR}/_ext/1376093119/sys_command.o ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o ${OBJECTDIR}/_ext/1832805299/sys_console.o ${OBJECTDIR}/_ext/1881668453/sys_int.o ${OBJECTDIR}/_ext/1000052432/sys_reset.o ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o ${OBJECTDIR}/_ext/308758920/usb_device.o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o ${OBJECTDIR}/_ext/1171490990/tasks.o ${OBJECTDIR}/_ext/246609638/port.o ${OBJECTDIR}/_ext/1665200909/heap_1.o ${OBJECTDIR}/_ext/404212886/timers.o ${OBJECTDIR}/_ext/404212886/list.o ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o ${OBJECTDIR}/_ext/404212886/queue.o ${OBJECTDIR}/_ext/404212886/croutine.o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ${OBJECTDIR}/_ext/404212886/event_groups.o ${OBJECTDIR}/startup_samd51j19a.o ${OBJECTDIR}/system_samd51j19a.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/app.o

# Source Files
SOURCEFILES=../src/config/default/driver/usart/src/drv_usart.c ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1.c ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1_device.c ../src/config/default/osal/osal_freertos.c ../src/config/default/peripheral/clock/plib_clock.c ../src/config/default/peripheral/cmcc/plib_cmcc.c ../src/config/default/peripheral/dmac/plib_dmac.c ../src/config/default/peripheral/evsys/plib_evsys.c ../src/config/default/peripheral/nvic/plib_nvic.c ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/default/peripheral/port/plib_port.c ../src/config/default/peripheral/sercom/usart/plib_sercom4_usart.c ../src/config/default/peripheral/sercom/usart/plib_sercom5_usart.c ../src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c ../src/config/default/peripheral/tc/plib_tc0.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/system/cache/sys_cache.c ../src/config/default/system/command/src/sys_command.c ../src/config/default/system/console/src/sys_console_usb_cdc.c ../src/config/default/system/console/src/sys_console.c ../src/config/default/system/int/src/sys_int.c ../src/config/default/system/reset/sys_reset.c ../src/config/default/usb/src/usb_device_cdc.c ../src/config/default/usb/src/usb_device_cdc_acm.c ../src/config/default/usb/src/usb_device.c ../src/config/default/startup_xc32.c ../src/config/default/exceptions.c ../src/config/default/initialization.c ../src/config/default/interrupts.c ../src/config/default/libc_syscalls.c ../src/config/default/usb_device_init_data.c ../src/config/default/freertos_hooks.c ../src/config/default/tasks.c ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F/port.c ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c ../src/third_party/rtos/FreeRTOS/Source/timers.c ../src/third_party/rtos/FreeRTOS/Source/list.c ../src/third_party/rtos/FreeRTOS/Source/FreeRTOS_tasks.c ../src/third_party/rtos/FreeRTOS/Source/queue.c ../src/third_party/rtos/FreeRTOS/Source/croutine.c ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c ../src/third_party/rtos/FreeRTOS/Source/event_groups.c startup_samd51j19a.c system_samd51j19a.c ../src/main.c ../src/app.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${DFP_DIR}/samd51a/include"  -I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=SAMD51J19A
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/869808652/drv_usart.o: ../src/config/default/driver/usart/src/drv_usart.c  .generated_files/flags/default/524609242cd774ddc622185fce5a420be047d366 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/869808652" 
	@${RM} ${OBJECTDIR}/_ext/869808652/drv_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/869808652/drv_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/869808652/drv_usart.o.d" -o ${OBJECTDIR}/_ext/869808652/drv_usart.o ../src/config/default/driver/usart/src/drv_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o: ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1.c  .generated_files/flags/default/f81dbe91d31c592d5ead6a7ceb3efdb761970bba .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/818654064" 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o.d 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o.d" -o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o: ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1_device.c  .generated_files/flags/default/f880a3f41d2b99a678f109c3d099041a6ffc548e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/818654064" 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o.d" -o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1_device.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1529399856/osal_freertos.o: ../src/config/default/osal/osal_freertos.c  .generated_files/flags/default/3acbf79a673279fe3b3fcea9bcc053b894404c1a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1529399856" 
	@${RM} ${OBJECTDIR}/_ext/1529399856/osal_freertos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1529399856/osal_freertos.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1529399856/osal_freertos.o.d" -o ${OBJECTDIR}/_ext/1529399856/osal_freertos.o ../src/config/default/osal/osal_freertos.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1984496892/plib_clock.o: ../src/config/default/peripheral/clock/plib_clock.c  .generated_files/flags/default/1d324b9a5dd44b72a8ac4b2e4b30f7eb8389121d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1984496892" 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ../src/config/default/peripheral/clock/plib_clock.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865131932/plib_cmcc.o: ../src/config/default/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/default/a171f337dab3d456d0a281b6bd05ccc27a5698f4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865131932" 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ../src/config/default/peripheral/cmcc/plib_cmcc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865161661/plib_dmac.o: ../src/config/default/peripheral/dmac/plib_dmac.c  .generated_files/flags/default/45d91cdffba37a0e051888e812149c0426334091 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865161661" 
	@${RM} ${OBJECTDIR}/_ext/1865161661/plib_dmac.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865161661/plib_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865161661/plib_dmac.o.d" -o ${OBJECTDIR}/_ext/1865161661/plib_dmac.o ../src/config/default/peripheral/dmac/plib_dmac.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1986646378/plib_evsys.o: ../src/config/default/peripheral/evsys/plib_evsys.c  .generated_files/flags/default/f2dda3ba12a3918495821126428a7c396a5653f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1986646378" 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ../src/config/default/peripheral/evsys/plib_evsys.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865468468/plib_nvic.o: ../src/config/default/peripheral/nvic/plib_nvic.c  .generated_files/flags/default/8323e01392496095d76f2888f6c9362a4da00f1a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865468468" 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ../src/config/default/peripheral/nvic/plib_nvic.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o: ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/default/b567b122cd2ab7bf9a633140ede905b9b6844749 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1593096446" 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865521619/plib_port.o: ../src/config/default/peripheral/port/plib_port.c  .generated_files/flags/default/31cb1bf90b97fc869c63b83e07cd11ba9aec1c58 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865521619" 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" -o ${OBJECTDIR}/_ext/1865521619/plib_port.o ../src/config/default/peripheral/port/plib_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom4_usart.c  .generated_files/flags/default/88f885278c241e9afd7eff569cf0d484fdb0d98a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom4_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom5_usart.c  .generated_files/flags/default/ed857bb5e356c5716f58d1ada6dad612012fdca6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom5_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c  .generated_files/flags/default/e5d93f0014241da4a31ce74d16e2bba01e9673db .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/829342655/plib_tc0.o: ../src/config/default/peripheral/tc/plib_tc0.c  .generated_files/flags/default/5d0e1c748dc02ce6e8564a07d4ad285a33900338 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/829342655" 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o.d 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/829342655/plib_tc0.o.d" -o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ../src/config/default/peripheral/tc/plib_tc0.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/4ea7142c76ca31a251a8211e9952506da1e7237e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1014039709/sys_cache.o: ../src/config/default/system/cache/sys_cache.c  .generated_files/flags/default/6ec74d0e00cf557aa184aa22cb0cc8e1f3a91e45 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1014039709" 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o.d 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1014039709/sys_cache.o.d" -o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ../src/config/default/system/cache/sys_cache.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1376093119/sys_command.o: ../src/config/default/system/command/src/sys_command.c  .generated_files/flags/default/91e221c6955f58ddf772c92c28a329f669d2d1fe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1376093119" 
	@${RM} ${OBJECTDIR}/_ext/1376093119/sys_command.o.d 
	@${RM} ${OBJECTDIR}/_ext/1376093119/sys_command.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1376093119/sys_command.o.d" -o ${OBJECTDIR}/_ext/1376093119/sys_command.o ../src/config/default/system/command/src/sys_command.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o: ../src/config/default/system/console/src/sys_console_usb_cdc.c  .generated_files/flags/default/a752e224d8d3d9c8ed7bddf54efc7b8a139ef718 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1832805299" 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o.d" -o ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o ../src/config/default/system/console/src/sys_console_usb_cdc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1832805299/sys_console.o: ../src/config/default/system/console/src/sys_console.c  .generated_files/flags/default/7aec2fb5f31659c166ae0f825182330f4d416c04 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1832805299" 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console.o.d 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1832805299/sys_console.o.d" -o ${OBJECTDIR}/_ext/1832805299/sys_console.o ../src/config/default/system/console/src/sys_console.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1881668453/sys_int.o: ../src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/f8724f10e4329c83876a7dde5a2a6213f8e9eedf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1881668453" 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1881668453/sys_int.o.d" -o ${OBJECTDIR}/_ext/1881668453/sys_int.o ../src/config/default/system/int/src/sys_int.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1000052432/sys_reset.o: ../src/config/default/system/reset/sys_reset.c  .generated_files/flags/default/d8baff24b955707162e6ac5cf1488bc34cd84eb7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1000052432" 
	@${RM} ${OBJECTDIR}/_ext/1000052432/sys_reset.o.d 
	@${RM} ${OBJECTDIR}/_ext/1000052432/sys_reset.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1000052432/sys_reset.o.d" -o ${OBJECTDIR}/_ext/1000052432/sys_reset.o ../src/config/default/system/reset/sys_reset.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/308758920/usb_device_cdc.o: ../src/config/default/usb/src/usb_device_cdc.c  .generated_files/flags/default/9630c30b62f29ffafde7482965e923832adc681c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/308758920" 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/308758920/usb_device_cdc.o.d" -o ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o ../src/config/default/usb/src/usb_device_cdc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o: ../src/config/default/usb/src/usb_device_cdc_acm.c  .generated_files/flags/default/6f2a9f1f448e778f9e7a10134f2ddc32c734520c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/308758920" 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o.d 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o.d" -o ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o ../src/config/default/usb/src/usb_device_cdc_acm.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/308758920/usb_device.o: ../src/config/default/usb/src/usb_device.c  .generated_files/flags/default/510a9f09894eb6dd112f1cb61ae64df8f8f962a3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/308758920" 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/308758920/usb_device.o.d" -o ${OBJECTDIR}/_ext/308758920/usb_device.o ../src/config/default/usb/src/usb_device.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/startup_xc32.o: ../src/config/default/startup_xc32.c  .generated_files/flags/default/691ddb7d5e4bd149934b9721601f8229ad55bee7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ../src/config/default/startup_xc32.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  .generated_files/flags/default/9a483b45596810c282b77bcc260f72686b40b811 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  .generated_files/flags/default/75bf8399001ab0268589bbe6e064f503e3b72e7a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  .generated_files/flags/default/f0261bfa7dedc81e8cf41b47e119d840d197e7d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/libc_syscalls.o: ../src/config/default/libc_syscalls.c  .generated_files/flags/default/5d2ad5602b3d2b47580773bcb24491ea8b9291ea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ../src/config/default/libc_syscalls.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o: ../src/config/default/usb_device_init_data.c  .generated_files/flags/default/ce87fec3efca034a8e8cf9d39eee00f0b0ee7665 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o.d" -o ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o ../src/config/default/usb_device_init_data.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/freertos_hooks.o: ../src/config/default/freertos_hooks.c  .generated_files/flags/default/f945dbf34c670002aa1490cc20263da2a344f046 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/freertos_hooks.o.d" -o ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o ../src/config/default/freertos_hooks.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/tasks.o: ../src/config/default/tasks.c  .generated_files/flags/default/95d75217827406f58778c2ac40a06db657ae0337 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/tasks.o.d" -o ${OBJECTDIR}/_ext/1171490990/tasks.o ../src/config/default/tasks.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/246609638/port.o: ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F/port.c  .generated_files/flags/default/c9a605cfd1e579e1f121b1cebe2aa1ddaa9105a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/246609638" 
	@${RM} ${OBJECTDIR}/_ext/246609638/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/246609638/port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/246609638/port.o.d" -o ${OBJECTDIR}/_ext/246609638/port.o ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F/port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1665200909/heap_1.o: ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c  .generated_files/flags/default/444967793243cf0f5935025eb69a02c5d753632a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1665200909" 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1665200909/heap_1.o.d" -o ${OBJECTDIR}/_ext/1665200909/heap_1.o ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/timers.o: ../src/third_party/rtos/FreeRTOS/Source/timers.c  .generated_files/flags/default/8be5d87db98dda6a24efb02f8a81b0bbcaaf3a90 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/timers.o.d" -o ${OBJECTDIR}/_ext/404212886/timers.o ../src/third_party/rtos/FreeRTOS/Source/timers.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/list.o: ../src/third_party/rtos/FreeRTOS/Source/list.c  .generated_files/flags/default/8a514e763082b7f5ca0d99f1d761e8a33011a6d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/list.o.d" -o ${OBJECTDIR}/_ext/404212886/list.o ../src/third_party/rtos/FreeRTOS/Source/list.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o: ../src/third_party/rtos/FreeRTOS/Source/FreeRTOS_tasks.c  .generated_files/flags/default/ade9743a37594637af8871f5f02b74dac8a71a5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o.d" -o ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o ../src/third_party/rtos/FreeRTOS/Source/FreeRTOS_tasks.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/queue.o: ../src/third_party/rtos/FreeRTOS/Source/queue.c  .generated_files/flags/default/3e72f63b93a972da605b5cf4d65f82ab4304864e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/queue.o.d" -o ${OBJECTDIR}/_ext/404212886/queue.o ../src/third_party/rtos/FreeRTOS/Source/queue.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/croutine.o: ../src/third_party/rtos/FreeRTOS/Source/croutine.c  .generated_files/flags/default/d228b98066ea3551091c9f518a675445130fc9e0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/croutine.o.d" -o ${OBJECTDIR}/_ext/404212886/croutine.o ../src/third_party/rtos/FreeRTOS/Source/croutine.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/stream_buffer.o: ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c  .generated_files/flags/default/87426448c6d4a4179f261ce253b14a8f54f7b9fb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/stream_buffer.o.d" -o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/event_groups.o: ../src/third_party/rtos/FreeRTOS/Source/event_groups.c  .generated_files/flags/default/600b5f5a49d97481fdada08bfcdb0c89df057b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/event_groups.o.d" -o ${OBJECTDIR}/_ext/404212886/event_groups.o ../src/third_party/rtos/FreeRTOS/Source/event_groups.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/startup_samd51j19a.o: startup_samd51j19a.c  .generated_files/flags/default/1b7821878404a89b0c632cc85ce9ae31efb7386 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51j19a.o.d" -o ${OBJECTDIR}/startup_samd51j19a.o startup_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/system_samd51j19a.o: system_samd51j19a.c  .generated_files/flags/default/2f96ee9d398f1e5649ad9b1f4690b8ce6a427859 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51j19a.o.d" -o ${OBJECTDIR}/system_samd51j19a.o system_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/cbe66e205ae524d0eb85ef692f4444dae28cfdda .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  .generated_files/flags/default/beb82cea64cd17496b1ac6399af0debf1f935d7e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/869808652/drv_usart.o: ../src/config/default/driver/usart/src/drv_usart.c  .generated_files/flags/default/d00cadb1e3e3689758eed5159ae90e409b0de47c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/869808652" 
	@${RM} ${OBJECTDIR}/_ext/869808652/drv_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/869808652/drv_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/869808652/drv_usart.o.d" -o ${OBJECTDIR}/_ext/869808652/drv_usart.o ../src/config/default/driver/usart/src/drv_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o: ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1.c  .generated_files/flags/default/df83f0b040ba3005c1701b55acbdd87254c17845 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/818654064" 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o.d 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o.d" -o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1.o ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o: ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1_device.c  .generated_files/flags/default/e8e467024297b05c348838f624789796feb31f95 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/818654064" 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o.d" -o ${OBJECTDIR}/_ext/818654064/drv_usbfsv1_device.o ../src/config/default/driver/usb/usbfsv1/src/drv_usbfsv1_device.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1529399856/osal_freertos.o: ../src/config/default/osal/osal_freertos.c  .generated_files/flags/default/dc7dd42e8283be325bafeaf69402977b34cd3b7e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1529399856" 
	@${RM} ${OBJECTDIR}/_ext/1529399856/osal_freertos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1529399856/osal_freertos.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1529399856/osal_freertos.o.d" -o ${OBJECTDIR}/_ext/1529399856/osal_freertos.o ../src/config/default/osal/osal_freertos.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1984496892/plib_clock.o: ../src/config/default/peripheral/clock/plib_clock.c  .generated_files/flags/default/48e4c4eb4f07650cb4fe58a4ad8e8402b28a203a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1984496892" 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ../src/config/default/peripheral/clock/plib_clock.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865131932/plib_cmcc.o: ../src/config/default/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/default/4c16b9ada594beb872c43262650048c7ab203573 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865131932" 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ../src/config/default/peripheral/cmcc/plib_cmcc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865161661/plib_dmac.o: ../src/config/default/peripheral/dmac/plib_dmac.c  .generated_files/flags/default/373c04608e884e434b61fcd24b28e4944619727e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865161661" 
	@${RM} ${OBJECTDIR}/_ext/1865161661/plib_dmac.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865161661/plib_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865161661/plib_dmac.o.d" -o ${OBJECTDIR}/_ext/1865161661/plib_dmac.o ../src/config/default/peripheral/dmac/plib_dmac.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1986646378/plib_evsys.o: ../src/config/default/peripheral/evsys/plib_evsys.c  .generated_files/flags/default/f35cc5a7f6bf466c59a65981b9f56521379ed9b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1986646378" 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ../src/config/default/peripheral/evsys/plib_evsys.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865468468/plib_nvic.o: ../src/config/default/peripheral/nvic/plib_nvic.c  .generated_files/flags/default/e3731894b4b6d9ab9b6b2ec2f5b424412b0d6895 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865468468" 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ../src/config/default/peripheral/nvic/plib_nvic.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o: ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/default/5d25b271ed5cfcae5e681dbfe02899550916db5b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1593096446" 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1865521619/plib_port.o: ../src/config/default/peripheral/port/plib_port.c  .generated_files/flags/default/3b7540ee09a9e5c5ad21f267683d31f7a7babbc4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865521619" 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" -o ${OBJECTDIR}/_ext/1865521619/plib_port.o ../src/config/default/peripheral/port/plib_port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom4_usart.c  .generated_files/flags/default/4e6e217083176cd323d59e59044b968b5016300c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom4_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom4_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom5_usart.c  .generated_files/flags/default/972d2e7935ed98adab4745aec40b96dde200538c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom5_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom5_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c  .generated_files/flags/default/7dd660fe5fbfbf7d224b97957b4214d0ec1f6486 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom1_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/829342655/plib_tc0.o: ../src/config/default/peripheral/tc/plib_tc0.c  .generated_files/flags/default/af46409f09742abea7130269b084d0b96b47a51f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/829342655" 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o.d 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/829342655/plib_tc0.o.d" -o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ../src/config/default/peripheral/tc/plib_tc0.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/8ca802ac9079705a7063ab3bc52ea0a1c71814ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1014039709/sys_cache.o: ../src/config/default/system/cache/sys_cache.c  .generated_files/flags/default/adc9d7da8ac03760d9f77be555fc58a9d61ec38f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1014039709" 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o.d 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1014039709/sys_cache.o.d" -o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ../src/config/default/system/cache/sys_cache.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1376093119/sys_command.o: ../src/config/default/system/command/src/sys_command.c  .generated_files/flags/default/47b9807ace8b64fb4c55e5505b0f0813d9b2d759 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1376093119" 
	@${RM} ${OBJECTDIR}/_ext/1376093119/sys_command.o.d 
	@${RM} ${OBJECTDIR}/_ext/1376093119/sys_command.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1376093119/sys_command.o.d" -o ${OBJECTDIR}/_ext/1376093119/sys_command.o ../src/config/default/system/command/src/sys_command.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o: ../src/config/default/system/console/src/sys_console_usb_cdc.c  .generated_files/flags/default/9b004166be5556229c354c14a7d60e5503d0e403 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1832805299" 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o.d" -o ${OBJECTDIR}/_ext/1832805299/sys_console_usb_cdc.o ../src/config/default/system/console/src/sys_console_usb_cdc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1832805299/sys_console.o: ../src/config/default/system/console/src/sys_console.c  .generated_files/flags/default/600bcad9e8e26939ac5c8057e9c7a267409a6829 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1832805299" 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console.o.d 
	@${RM} ${OBJECTDIR}/_ext/1832805299/sys_console.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1832805299/sys_console.o.d" -o ${OBJECTDIR}/_ext/1832805299/sys_console.o ../src/config/default/system/console/src/sys_console.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1881668453/sys_int.o: ../src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/b7dbbe442919b7cdf970f57b0a11bbf75b73d9f0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1881668453" 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1881668453/sys_int.o.d" -o ${OBJECTDIR}/_ext/1881668453/sys_int.o ../src/config/default/system/int/src/sys_int.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1000052432/sys_reset.o: ../src/config/default/system/reset/sys_reset.c  .generated_files/flags/default/745c2d66df4fa1e1c9a8d930ab4cb6018c05712c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1000052432" 
	@${RM} ${OBJECTDIR}/_ext/1000052432/sys_reset.o.d 
	@${RM} ${OBJECTDIR}/_ext/1000052432/sys_reset.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1000052432/sys_reset.o.d" -o ${OBJECTDIR}/_ext/1000052432/sys_reset.o ../src/config/default/system/reset/sys_reset.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/308758920/usb_device_cdc.o: ../src/config/default/usb/src/usb_device_cdc.c  .generated_files/flags/default/ab457f610ed392a683bd43514dbc704c41d89918 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/308758920" 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/308758920/usb_device_cdc.o.d" -o ${OBJECTDIR}/_ext/308758920/usb_device_cdc.o ../src/config/default/usb/src/usb_device_cdc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o: ../src/config/default/usb/src/usb_device_cdc_acm.c  .generated_files/flags/default/d7165f37067963e384ad5567bb736d258111a46a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/308758920" 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o.d 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o.d" -o ${OBJECTDIR}/_ext/308758920/usb_device_cdc_acm.o ../src/config/default/usb/src/usb_device_cdc_acm.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/308758920/usb_device.o: ../src/config/default/usb/src/usb_device.c  .generated_files/flags/default/d24e9442e73a6bef5de3c46a29d4ef5121660a9b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/308758920" 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/308758920/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/308758920/usb_device.o.d" -o ${OBJECTDIR}/_ext/308758920/usb_device.o ../src/config/default/usb/src/usb_device.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/startup_xc32.o: ../src/config/default/startup_xc32.c  .generated_files/flags/default/1e038d64b8265390045ee5bb5b11de8f48e817b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ../src/config/default/startup_xc32.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  .generated_files/flags/default/f7abdfcb2901dd761923f2647cedd86f4f67329c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  .generated_files/flags/default/86604a8b50488d34349ee56903a0956e8107dc98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  .generated_files/flags/default/6757aaad8826724833dba8254150d9fffc7003a2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/libc_syscalls.o: ../src/config/default/libc_syscalls.c  .generated_files/flags/default/3029c965bdc11b0295156db669f0df1f444086e6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ../src/config/default/libc_syscalls.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o: ../src/config/default/usb_device_init_data.c  .generated_files/flags/default/31eaf01db5f5105f6eebb9e4c7cce3a649f93c45 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o.d" -o ${OBJECTDIR}/_ext/1171490990/usb_device_init_data.o ../src/config/default/usb_device_init_data.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/freertos_hooks.o: ../src/config/default/freertos_hooks.c  .generated_files/flags/default/2d5caefd1cd88430a1755f8020222c3b04d268de .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/freertos_hooks.o.d" -o ${OBJECTDIR}/_ext/1171490990/freertos_hooks.o ../src/config/default/freertos_hooks.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1171490990/tasks.o: ../src/config/default/tasks.c  .generated_files/flags/default/22b170b6f448ed129bedfce3120bfbfa23a22b79 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/tasks.o.d" -o ${OBJECTDIR}/_ext/1171490990/tasks.o ../src/config/default/tasks.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/246609638/port.o: ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F/port.c  .generated_files/flags/default/3e93f001646a70c2ec6cbe46ca8fd6899101c1ee .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/246609638" 
	@${RM} ${OBJECTDIR}/_ext/246609638/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/246609638/port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/246609638/port.o.d" -o ${OBJECTDIR}/_ext/246609638/port.o ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F/port.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1665200909/heap_1.o: ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c  .generated_files/flags/default/bd23848dd2af1ba94a3c7fdfee76ca365222846b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1665200909" 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1665200909/heap_1.o.d" -o ${OBJECTDIR}/_ext/1665200909/heap_1.o ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/timers.o: ../src/third_party/rtos/FreeRTOS/Source/timers.c  .generated_files/flags/default/90e906b8f5d030199ecafa9009fdb2942ca8faee .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/timers.o.d" -o ${OBJECTDIR}/_ext/404212886/timers.o ../src/third_party/rtos/FreeRTOS/Source/timers.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/list.o: ../src/third_party/rtos/FreeRTOS/Source/list.c  .generated_files/flags/default/832dd923b98e14488e3801798be1623d4a76889d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/list.o.d" -o ${OBJECTDIR}/_ext/404212886/list.o ../src/third_party/rtos/FreeRTOS/Source/list.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o: ../src/third_party/rtos/FreeRTOS/Source/FreeRTOS_tasks.c  .generated_files/flags/default/db608d5730308cf94a07760f9446c6370ec4a3a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o.d" -o ${OBJECTDIR}/_ext/404212886/FreeRTOS_tasks.o ../src/third_party/rtos/FreeRTOS/Source/FreeRTOS_tasks.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/queue.o: ../src/third_party/rtos/FreeRTOS/Source/queue.c  .generated_files/flags/default/ce51a307d6ee86f3f4edcc2d7195b11cdbb655ef .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/queue.o.d" -o ${OBJECTDIR}/_ext/404212886/queue.o ../src/third_party/rtos/FreeRTOS/Source/queue.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/croutine.o: ../src/third_party/rtos/FreeRTOS/Source/croutine.c  .generated_files/flags/default/619566d96cc8bde8207fd61794936048a576aac2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/croutine.o.d" -o ${OBJECTDIR}/_ext/404212886/croutine.o ../src/third_party/rtos/FreeRTOS/Source/croutine.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/stream_buffer.o: ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c  .generated_files/flags/default/e5903244de431f423f60170135e117d151e1057f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/stream_buffer.o.d" -o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/event_groups.o: ../src/third_party/rtos/FreeRTOS/Source/event_groups.c  .generated_files/flags/default/9280e68a858256641423f72ac7607fe91cbceb2d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/404212886/event_groups.o.d" -o ${OBJECTDIR}/_ext/404212886/event_groups.o ../src/third_party/rtos/FreeRTOS/Source/event_groups.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/startup_samd51j19a.o: startup_samd51j19a.c  .generated_files/flags/default/6b9aa5367f1d5a49a23939470cfbd13144563420 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51j19a.o.d" -o ${OBJECTDIR}/startup_samd51j19a.o startup_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/system_samd51j19a.o: system_samd51j19a.c  .generated_files/flags/default/ddb69d97cf598836a4bbe24e36ab4fed2eeed76 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51j19a.o.d" -o ${OBJECTDIR}/system_samd51j19a.o system_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/368910651cc8b1faa6763b36725b30db7c1136c7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  .generated_files/flags/default/9ff482460424f3682387fadf77ecb5756f5c5fa7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    samd51j19a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4   -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__   -T"samd51j19a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}\BetSpotHostSim.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections  
	
	
	
	
	
	
else
${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   samd51j19a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4  -D__$(MP_PROCESSOR_OPTION)__   -T"samd51j19a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}\BetSpotHostSim.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections  
	
	${MP_CC_DIR}\\arm-none-eabi-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/BetSpotHostSim.X.${IMAGE_TYPE}.hex"
	
	
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
