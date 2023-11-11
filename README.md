# TFT_eSPI Table
A wrapper that makes it easier to create tables using the TFT-eSPI library

# Disclaimer
- This table is 'static' in nature. While it is possible to alter its internal dimensions (column/row sizes before generation), the number of columns/rows remains fixed at runtime.
- Generating the table, altering its stacture (for example using table->setRowHeight()) and then calling generate again, has not been tested. For predictable results follow the examples bellow.

# Types
- TableHeap: all the internal buffers are heap allocated, as such, dimentions can be specified at runtime.
- TableStack: all the internal buffers are stack allocated, more efficient, but dimentions must be specified at compile time.

# Examples
## 2x3
```cpp
#include "TableHeap.h"

auto x = 0;
auto y = 0;
auto width = 480;
auto height = 320;
auto rows = 2;
auto cols = 3;

TableHeap* table = new TableHeap(
    tft,
    x,
    y,
    width,
    height,
    rows,
    cols,
    TFT_WHITE,
    TFT_BLACK
);

table->generate();
table->draw();
```
<div align="center">
  <img src="https://github.com/xarDeves/TFT_eSPI-Table/blob/master/images/2x3.jpg" alt="table->2x3">
</div>

## 4x4 custom row/column
```cpp
#include "TableHeap.h"

auto x = 0;
auto y = 0;
auto width = 480;
auto height = 320;
auto rows = 4;
auto cols = 4;

TableHeap* table = new TableHeap(
    tft,
    x,
    y,
    width,
    height,
    rows,
    cols,
    TFT_WHITE,
    TFT_BLACK
);

table->setRowHeight(0, 30);
table->setColumnWidth(0, 30);

table->generate();
table->draw();
```
<div align="center">
  <img src="https://github.com/xarDeves/TFT_eSPI-Table/blob/master/images/4x4_custom.jpg" alt="table->4x4_custom">
</div>

## 1x4 cell inset
```cpp
#include "TableHeap.h"

auto x = 0;
auto y = 0;
auto width = 480;
auto height = 320;
auto rows = 3;
auto cols = 9;
auto cellInset = 8;

TableHeap* table = new TableHeap(
    tft,
    x,
    y,
    width,
    height,
    rows,
    cols,
    TFT_BLACK,
    TFT_BLACK
);

table->setRowHeight(0, cellInset);
table->setRowHeight(2, cellInset);

table->setColumnWidth(0, cellInset);
table->setColumnWidth(2, cellInset);
table->setColumnWidth(4, cellInset);
table->setColumnWidth(6, cellInset);
table->setColumnWidth(8, cellInset);

table->setCellOutlineColor(1, 1, TFT_WHITE);
table->setCellOutlineColor(1, 3, TFT_WHITE);
table->setCellOutlineColor(1, 5, TFT_WHITE);
table->setCellOutlineColor(1, 7, TFT_WHITE);

table->generate();
table->draw();
```
<div align="center">
  <img src="https://github.com/xarDeves/TFT_eSPI-Table/blob/master/images/1x3_insets.jpg" alt="table->1x4_inset">
</div>

## Styling
stylizing the cells' outline and/or background colors.
```cpp
table->setCellOutlineColor(row, column, TFT_RED);
table->setCellFillColor(row, column, TFT_BLUE);
//table must always be generated first (table->genarate())
table->draw();
```

## Text
```cpp
//table must always be generated first (table->genarate())
table->drawCellText(row, column, String(x), TFT_GREEN);
```

## Erase
A cell's contents can be erased (by redrawing the cell's background color on top of everything).
```cpp
//table must always be generated first (table->genarate())
table->eraseCell(row, column);
```

## TableStack
Same usage as TableHeap (pay attention to the disclamer above)

In your .h file:
```cpp
#include "TableStack.h"

constexpr auto x = 0;
constexpr auto y = 0;
constexpr auto width = 480;
constexpr auto height = 320;
constexpr auto rows = 2;
constexpr auto cols = 2;
 
class MyClass{  
    TableStack<
        x,
        y,
        width,
        static_cast<int>(height / 2.5),
        rows,
        cols
    > table;
};
```
In your .cpp file:
```cpp
MyClass::MyClass() : table(tft, TFT_WHITE, TFT_BLACK) {
    table.generate();
    table.draw();
}
```