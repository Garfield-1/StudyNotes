LIBS := -L./depends/depend_libs -lubus -lubusd_library -lblobmsg_json -lubox
CLAGS := -g -o

all:
	gcc test.c $(CLAGS) a.out $(LIBS) -I./depends/depend_inc

clean:
	rm a.out