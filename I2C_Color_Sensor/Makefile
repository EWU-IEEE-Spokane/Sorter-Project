all: I2C.bin

%.o: %.c
	@echo arm-none-eabi-gcc \
		-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 \
		-mfloat-abi=softfp -ffunction-sections \
		-fdata-sections -MD -std=c99 -Wall \
		-pedantic -c -o ${@} ${<}
	@arm-none-eabi-gcc -ggdb \
		-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 \
		-mfloat-abi=softfp -ffunction-sections \
		-fdata-sections -MD -std=c99 -Wall \
		-pedantic -c -o ${@} ${<}

I2C.axf: tiva.ld
I2C.axf: startup.o
I2C.axf: i2c.o
I2C.axf: led.o
I2C.axf: color_sensor.o
I2C.axf: color_process.o
I2C.axf: uart.o

%.axf:
	@echo arm-none-eabi-ld \
	       -T tiva.ld --entry ResetISR --gc-sections \
	       -o ${@} $(filter %.o %.a, ${^})
	@arm-none-eabi-ld \
	       -T tiva.ld --entry ResetISR \
	       -o ${@} $(filter %.o %.a, ${^})


%.bin: %.axf
	@echo arm-none-eabi-objcopy -O binary ${@:.bin=.axf} ${@}
	@arm-none-eabi-objcopy -O binary ${@:.bin=.axf} ${@}


clean:
	@rm -rf *.bin *.axf *.o *.d
