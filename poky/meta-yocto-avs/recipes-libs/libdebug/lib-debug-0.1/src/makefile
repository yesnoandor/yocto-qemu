##############################################################################################
# $ gcc -c func.c -o func.o : 编译生成obj文件
# $ gcc -c exe -o func.o   :  编译生成可执行文件
# $ ar rc libfunc.a func.o :  编译生成静态库
# $ gcc -shared -o libxxx.so aaa.o bbb.o :  编译生成动态库
#
# -I :     
# -L : 指定库的路径
# -l : 指定静态库的名称
# -Wl,R : 
#
# -g : 生成调试信息
# −Wall : 打开警告开关
# -O : 默认优化
# -fPIC : 于编译阶段,告诉编译器产生与位置无关代码
# -shared :  
#
# $< : 依赖目标中的第一个目标名字，如果依赖目标是以模式（即“%”）定义的，则表示符合模式的一系列文件集，其是一个一个去出来的
# $@ : 规则中的目标文件集
# $^ : 规则中所有的依赖目标的集合，用空格分隔
# $? : 所有比目标新的依赖目标的集合
#
# wildcard : 扩展通配符
# 
# include : 包含文件，
# -include : 忽略由于包含文件不存在或者无法创建时的错误提示 （同sinclude）
# 
#  = 是最基本的赋值
# := 是覆盖之前的值
# ?= 是如果没有被赋值过就赋予等号后面的值
# += 是添加等号后面的值
#
# Makefile常用函数
# 1. 字符串处理函数
#	subst ： 字符串替换符		$(subst <from>;,<to>;,<text>;) : 把字串<text>;中的<from>;字符串替换成<to>;
#	patsubst : 替换通配符 		
#		$(patsubst <pattern>,<replacement>,<text>) ： 查找<text>中的单词是否符合模式<pattern>，如果匹配，则<replacement>替换
#	strip ： 去空格				$(strip <string> ) ： 去掉<string>字串中开头和结尾的空字符
#	filter : 过滤函数			$(filter-out $(in),$(objects))	: 以模式过滤字符串中的单词，保留符合模式的单词
#	filter-out : 反过滤函数 	$(filter-out $(ex),$(objects)) ： 以模式过滤字符串中的单词，去除符合模式的单词
#
#
#
#
# 2. 文件处理函数
# 	dir :		取目录函数		$(dir src/foo.c hacks)	: 返回值是“src/ ./”
# 	notdir :	取文件函数		$(notdir src/foo.c hack) : 返回值是“foo.c hack”
#	suffix :	取后缀函数		$(suffix src/foo.c src-1.0/bar.c hacks) : 返回值是“.c .c”
#	basename :	取前缀函数		$(basenamesrc/foo.c src-1.0/bar.c hacks) : 返回值是“src/foo src-1.0/bar hacks”
#	addprefix : 加前缀函数		$(addprefix src/,foo bar) ： 返回值是	“src/foo src/bar”
#	addsuffix : 加后缀函数		$(addsuffix .c,foo bar)	： 返回值是	“foo.c bar.c”
#	join : 连接函数				$(join a b c d,1 2 3 4) : 返回值是 “a1 b2 c3 d4”
#
#
# 3. eval函数		$(eval text，arg1,arg2)
#	使用其它变量和函数,构造一个可变的规则结构关系（依赖关系链）
# 4. foreach函数 $(foreach  var, list, text)
#	重复利用一段脚本，循环执行
# 5. if 函数		$(if <condition>,<then-part> )  or $(if <condition>,<then-part>,<else-part> )
# 	条件判断，并执行一个分支
# 6. call函数	$(call <expression>,<parm1>,<parm2>,<parm3>...)
#	用来创建新的参数化的函数
# 7. shell函数
#
# 8. value函数
#	提供了一种使用变量非展开值的方式
# 9.
#
# Makefile中写shell代码
# 	1. 在Makefile文件的目标项冒号后的另起一行的代码才是shell代码 （shell代码中不允许‘=’号两边有空格）
#	2. Makefile中的shell，每一行是一个进程，不同行之间变量值不能传递 （用‘/’符号 将属于同一行的代码拆分为多行显示）
#	3. Makefile中的变量以$开头，shell的变量以$$开头 
#	4. @命令，命令本身不被显示出来
#	5. shell中的特别变量
#		$0 正在被执行命令的名字。对于shell脚本而言，这是被激活命令的路径
#		$n 该变量与脚本被激活时所带的参数相对应。n是正整数，与参数位置相对应($1,$2…)
#		$# shell中特殊变量
#		$* 所有这些参数都被双引号引住。若一个脚本接收两个参数，$*等于$1$2
#		$@ 所有这些参数都分别被双引号引住。若一个脚本接收到两个参数，$@等价于$1$2
#		$? 前一个命令执行后的退出状态
#		$$ 当前shell的进程号。对于shell脚本，这是其正在执行时的进程ID
#		$! 前一个后台命令的进程号
#	
#
#	
##############################################################################################
LIB = libdebug.so

SRC = $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRC))

%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) -Wall -fPIC $(CFLAGS) -c $< -o $@

#%.o: %.cpp
#	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
#	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

#$(TARGET).$(VERSION) : $(OBJS)
#	$(CC) -shared $< -o $@ $(LDFLAGS)

#$(TARGET): $(LIB).so.$(VERSION)
#	ln -s $< $@

$(LIB):  $(OBJS)
	$(CC) -shared $< -o $@ $(LDFLAGS)


all: $(LIB)


clean:
	rm -f $(OBJS) $(LIB)



