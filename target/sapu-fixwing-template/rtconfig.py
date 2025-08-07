import os

TARGET_NAME = 'sapu-fixwing'
# toolchains options
ARCH='arm'
CPU='cortex-m7'
CROSS_TOOL='gcc'

# bsp lib config
BSP_LIBRARY_TYPE = None

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')
if os.getenv('RTT_ROOT'):
    RTT_ROOT = os.getenv('RTT_ROOT')

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery, Keil MDK, IAR
if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = r'C:/EmbedDevTool/ARM-GNU-Toolchains/bin'
elif CROSS_TOOL == 'keil':
    PLATFORM    = 'armclang'
    EXEC_PATH   = r'D:/Keil_v5'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    CXX = PREFIX + 'g++'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    DEVICE = ' -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections'
    CFLAGS = DEVICE + ' -Dgcc'
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -Wa,-mimplicit-it=thumb '
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T board/linker_scripts/link.lds'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2 -g'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    CXXFLAGS = CFLAGS 
    CFLAGS += ' -std=c99'

    POST_ACTION = OBJCPY + ' -O binary $TARGET '+TARGET_NAME+'.bin\n' + SIZE + ' $TARGET \n'

elif PLATFORM == 'armclang':
    CC = 'armclang'
    CXX = 'armclang'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    CFLAGS = '-xc --target=arm-arm-none-eabi -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -c' \
            '-fno-rtti -funsigned-char -fshort-enums -fshort-wchar -gdwarf-4 -O1 ' \
            '-ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes ' \
            '-Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro ' \
            '-Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management ' \
            '-Wno-parentheses-equality -Wno-reserved-identifier'

    AFLAGS = r'--cpu Cortex-M7.fp.dp -g --apcs=interwork --list ".\build\keil\List\*.lst" --xref -o "*.o" --depend "*.d" '
    LFLAGS = r'--cpu Cortex-M7.fp.dp *.o --strict --scatter ".\board\linker_scripts\link.sct" --summary_stderr ' \
            r'--info summarysizes --map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused ' \
            r'--info veneers --list ".\build\keil\List\sapu-fixwing.map" -o .\build\keil\Obj\sapu-fixwing.axf'

    EXEC_PATH += '/ARM/ARMCLANG/bin/'

    CXXFLAGS = CFLAGS 

    TARGET_EXT = 'axf'
    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET'

elif PLATFORM == 'armcc':
    # toolchains
    CC = 'armcc'
    CXX = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --cpu Cortex-M7.fp.sp'
    CFLAGS = '-c ' + DEVICE + ' --apcs=interwork --c99'
    AFLAGS = DEVICE + ' --apcs=interwork '
    LFLAGS = DEVICE + ' --scatter "board\linker_scripts\link.sct" --info sizes --info totals --info unused --info veneers --list rtthread.map --strict'
    CFLAGS += ' -I' + EXEC_PATH + '/ARM/ARMCC/include'
    LFLAGS += ' --libpath=' + EXEC_PATH + '/ARM/ARMCC/lib'

    CFLAGS += ' -D__MICROLIB '
    AFLAGS += ' --pd "__MICROLIB SETA 1" '
    LFLAGS += ' --library_type=microlib '
    EXEC_PATH += '/ARM/ARMCC/bin/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -O2'

    CXXFLAGS = CFLAGS 
    CFLAGS += ' -std=c99'

    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET'


def dist_handle(BSP_ROOT, dist_dir):
    import sys
    cwd_path = os.getcwd()
    sys.path.append(os.path.join(os.path.dirname(BSP_ROOT), 'tools'))
    from sdk_dist import dist_do_building
    dist_do_building(BSP_ROOT, dist_dir)
