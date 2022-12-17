#ifndef Included_bytereader_h
#define Included_bytereader_h
#define ALTERNATE_COLOURS 01
#define OFFSET 02
#define SPACE_BYTES 04
#define CAPITAL_LETTERS 010
#define MASK_NUM(n, m)((n) & (m))
#define ISINTERACTIVE isatty(STDOUT_FILENO)
#endif
