CC = arm-linux-gcc #定义变量

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

IMAGE=main
COM_FLAG=-I . -lm


$(IMAGE):$(OBJ)
	$(CC) $(^) -o $(@) $(COM_FLAG)
	
$(OBJ):%.o:%.c
	$(CC) $(^) -o $(@) -c $(COM_FLAG)
	
clean:
	rm $(OBJ) $(IMAGE) -rf




