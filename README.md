# TFT_eSPI Table
A wrapper that makes it easier to create tables using the TFT-eSPI library

# Disclaimer
* This table is 'static' in nature. While it is possible to alter its dimensions, the number of columns/rows remains fixed at runtime.
* All of the internal data structures are dynamically allocated, plans for constexpr version are underway.

# Examples
## 2x3
```cpp
  auto x = 0;
  auto y = 0;
  auto width = 480;
  auto height = 320;

  Table* table = new Table(
      tft,
      x,
      y,
      width,
      height,
      2,
      3,
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
  auto x = 0;
  auto y = 0;
  auto width = 480;
  auto height = 320;

  Table* table = new Table(
      tft,
      x,
      y,
      width,
      height,
      4,
      4,
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

## 1x3 cell inset
```cpp
  auto x = 0;
  auto y = 0;
  auto width = 480;
  auto height = 320;
  auto cellInset = 8;

  Table* table = new Table(
      tft,
      x,
      y,
      width,
      height,
      3,
      9,
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

  table->generate();
  table->draw();
```
<div align="center">
  <img src="https://github.com/xarDeves/TFT_eSPI-Table/blob/master/images/1x3_insets.jpg" alt="table->1x3_insets">
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