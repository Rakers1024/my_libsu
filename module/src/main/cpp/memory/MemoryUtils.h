//获取进程pid
int get_pid_by_packagename(const char *szPro);
/* 读取模块入口地址 */
uintptr_t get_module_base(const char *module_name,int mPid);
uintptr_t getModule(const char *lib, bool isStart);
char *my_strcpy(char *dest, const char *src);
void *my_memmove(void *dst, const void *src, size_t count);
char *my_strcpy(char *dest, const char *src);
void writeBuffer(uintptr_t address, void *buffer, int size);
//缺页判断
int isMincore(uintptr_t _addr, size_t _size);
//内存读取
bool memoryRead(uintptr_t address, void *buffer, int size);
float getF(uintptr_t address);
uintptr_t getA(uintptr_t address);
char getChar(uintptr_t address);
int getI(uintptr_t address);
void writeFloat(uintptr_t address, float value);
void writeByte(uintptr_t address, int value);
void writeInt(uintptr_t address, int value);
//字节查找
uintptr_t FindPattern(const char *lib, const char *pattern);
int isMincoreSafeAddr(uintptr_t address);
int new_mincore(void* __addr, size_t __size, unsigned char *mincore_buffer);

void Hook(void *target, void *replace, void **backup);