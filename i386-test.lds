ENTRY(test)

SECTIONS
{
  . = 0x8048000 + SIZEOF_HEADERS;
  tinytest : { *(.text) *(.data) *(.rodata) }
  /DISCARD/ : { *(.comment) }
}
