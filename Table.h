#pragma once

#include "debug.h"

#include <SPI.h>
#include <TFT_eSPI.h>

class Table {
public:
  Table(TFT_eSPI *tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
        uint8_t rows, uint8_t columns, uint16_t outlineColor, uint16_t fillColor);

  void generate();
  void draw() const;

  void eraseCell(uint8_t row, uint8_t column) const;
  void drawCellText(uint8_t row, uint8_t column, String text, uint16_t clr) const;
  void drawCellOutline(uint8_t row, uint8_t column) const;

  void setCellFillColor(uint8_t row, uint8_t column, uint16_t color);
  void setCellOutlineColor(uint8_t row, uint8_t column, uint16_t color);

  void setRowHeight(uint8_t row, uint16_t height);
  void setColumnWidth(uint8_t column, uint16_t width);

  void setPaddingTop(uint16_t top);
  void setPaddingBottom(uint16_t bottom);
  void setPaddingLeft(uint16_t left);
  void setPaddingRight(uint16_t right);

  uint16_t getX() const;
  uint16_t getY() const;
  uint16_t getWidth() const;
  uint16_t getHeight() const;
  uint8_t getRows() const;
  uint8_t getColumns() const;

  ~Table();

private:
  TFT_eSPI *tft;

  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;

  uint16_t paddingTop;
  uint16_t paddingBottom;
  uint16_t paddingLeft;
  uint16_t paddingRight;

  uint8_t rows;
  uint8_t columns;
  uint16_t *columnWidths;
  uint16_t *rowHeights;
  uint16_t ***cellCenters;
  uint16_t** cellXCoordinates;
  uint16_t** cellYCoordinates;

  uint16_t **cellFillColors;
  uint16_t **cellOutlineColors;
  uint16_t outlineColor;
  uint16_t fillColor;

  void allocateArrays();
  void calculateColumnWidths();
  void calculateRowHeights();
  void calculateCellCenters();
  void calculateCellCoordinates();
};