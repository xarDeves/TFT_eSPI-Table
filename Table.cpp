#include "Table.h"

Table::Table(TFT_eSPI *tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
             uint8_t rows, uint8_t columns, uint16_t outlineColor, uint16_t fillColor)
    : tft(tft), x(x), y(y), paddingTop(0), paddingBottom(0), paddingLeft(0), paddingRight(0),
      rows(rows), columns(columns), outlineColor(outlineColor),
      fillColor(fillColor), width(width), height(height) {

  allocateArrays();
}

void Table::generate() {
  calculateColumnWidths();
  calculateRowHeights();
  calculateCellCoordinates();
  calculateCellCenters();
}

void Table::calculateColumnWidths() {
  auto remainingWidth = width - (paddingLeft + paddingRight);

  auto specifiedColumns = 0;
  for (auto i = 0; i < columns; i++) {
    if (columnWidths[i] != 0) {
      specifiedColumns++;
      remainingWidth -= columnWidths[i];
    }
  }

  float remainingColumns = columns - specifiedColumns;
  auto equalWidth = round(remainingWidth / remainingColumns);

  auto currentOffset = paddingLeft;
  for (auto i = 0; i < columns; i++) {
    if (columnWidths[i] == 0)
      columnWidths[i] = equalWidth;
    currentOffset += columnWidths[i];
  }
}

void Table::calculateRowHeights() {
  auto remainingHeight = height - (paddingTop + paddingBottom);

  auto specifiedRows = 0;
  for (auto i = 0; i < rows; i++) {
    if (rowHeights[i] != 0) {
      specifiedRows++;
      remainingHeight -= rowHeights[i];
    }
  }

  auto remainingRows = rows - specifiedRows;
  float equalHeight = round(remainingHeight / remainingRows);

  auto currentOffset = paddingTop;
  for (auto i = 0; i < rows; i++) {
    if (rowHeights[i] == 0)
      rowHeights[i] = equalHeight;
    currentOffset += rowHeights[i];
  }
}

void Table::calculateCellCoordinates() {
  auto currentX = x + paddingLeft;
  auto currentY = y + paddingTop;

  for (auto row = 0; row < rows; row++) {
    currentX = paddingLeft;
    for (auto column = 0; column < columns; column++) {
      cellXCoordinates[row][column] = currentX;
      currentX += columnWidths[column];
    }

    for (auto i = 0; i < columns; i++) {
      cellYCoordinates[row][i] = currentY;
    }

    currentY += rowHeights[row];
  }
}

void Table::calculateCellCenters() {
  for (auto row = 0; row < rows; row++) {
    auto trueY = y + paddingTop;
    for (auto i = 0; i < row; i++) {
      trueY += rowHeights[i];
    }

    for (auto column = 0; column < columns; column++) {
      auto x = cellXCoordinates[row][column] + columnWidths[column] / 2;
      cellCenters[row][column][0] = x;
      cellCenters[row][column][1] =  trueY + rowHeights[row] / 2;
    }
  }
}

void Table::draw() const {
  for (auto row = 0; row < rows; row++) {
    for (auto column = 0; column < columns; column++) {
      auto x = cellXCoordinates[row][column];
      auto y = cellYCoordinates[row][column];
      auto width = columnWidths[column];
      auto height = rowHeights[row];
      auto fillColor = cellFillColors[row][column];
      auto outlineColor = cellOutlineColors[row][column];

      tft->fillRect(x, y, width, height, fillColor);
      tft->drawRect(x, y, width, height, outlineColor);
    }
  }
}

void Table::drawCellText(uint8_t row, uint8_t column, String text, uint16_t clr) const {
  tft->setTextColor(clr);

  auto x = cellCenters[row][column][0];
  auto y = cellCenters[row][column][1];

  tft->drawString(text, x, y);

  tft->setTextColor(ILI9486_WHITE);
}

void Table::drawCellOutline(uint8_t row, uint8_t column) const {
  auto x = cellXCoordinates[row][column];
  auto y = cellYCoordinates[row][column];
  auto width = columnWidths[column];
  auto height = rowHeights[row];
  auto outlineColor = cellOutlineColors[row][column];

  tft->drawRect(x, y, width, height, outlineColor);
}

void Table::eraseCell(uint8_t row, uint8_t column) const {
  auto x = cellXCoordinates[row][column];
  auto y = cellYCoordinates[row][column];
  auto width = columnWidths[column];
  auto height = rowHeights[row];
  auto fillColor = cellFillColors[row][column];

  tft->fillRect(x + 1, y + 1, width - 2, height - 2, fillColor);
}

void Table::setCellFillColor(uint8_t row, uint8_t column, uint16_t color) {
  cellFillColors[row][column] = color;
}

void Table::setCellOutlineColor(uint8_t row, uint8_t column, uint16_t color) {
  cellOutlineColors[row][column] = color;
}

void Table::setRowHeight(uint8_t row, uint16_t height) {
  rowHeights[row] = height;
}

void Table::setColumnWidth(uint8_t column, uint16_t width) {
  columnWidths[column] = width;
}

void Table::setPaddingTop(uint16_t top) { paddingTop = top; }

void Table::setPaddingBottom(uint16_t bottom) { paddingBottom = bottom; }

void Table::setPaddingLeft(uint16_t left) { paddingLeft = left; }

void Table::setPaddingRight(uint16_t right) { paddingRight = right; }

uint16_t Table::getX() const {
  return x + paddingLeft;
}

uint16_t Table::getY() const {
  return y + paddingTop;
}

uint8_t Table::getRows() const {
  return rows;
}
uint8_t Table::getColumns() const {
  return columns;
}

uint16_t Table::getWidth() const {
  uint16_t width = 0;
  for (uint8_t column = 0; column < columns; column++) {
    width += columnWidths[column];
  }
  return width;
}

uint16_t Table::getHeight() const {
  uint16_t height = 0;
  for (uint8_t row = 0; row < rows; row++) {
    height += rowHeights[row];
  }
  return height;
}

void Table::allocateArrays() {
  columnWidths = new uint16_t[columns];
  rowHeights = new uint16_t[rows];
  cellCenters = new uint16_t **[rows];
  cellFillColors = new uint16_t *[rows];
  cellOutlineColors = new uint16_t *[rows];
  cellXCoordinates = new uint16_t *[rows];
  cellYCoordinates = new uint16_t *[rows];

  memset(columnWidths, 0, sizeof(uint16_t) * columns);
  memset(rowHeights, 0, sizeof(uint16_t) * rows);

  for (auto i = 0; i < rows; i++) {
    cellCenters[i] = new uint16_t *[columns];
    cellFillColors[i] = new uint16_t[columns];
    cellOutlineColors[i] = new uint16_t[columns];
    cellXCoordinates[i] = new uint16_t[columns];
    cellYCoordinates[i] = new uint16_t[columns];

    for (auto j = 0; j < columns; j++) {
      cellCenters[i][j] = new uint16_t[2];
      cellFillColors[i][j] = fillColor;
      cellOutlineColors[i][j] = outlineColor;
    }
  }
}

Table::~Table() {
  for (auto i = 0; i < rows; i++) {
    for (auto j = 0; j < columns; j++) {
      delete[] cellCenters[i][j];
    }
    delete[] cellCenters[i];
    delete[] cellFillColors[i];
    delete[] cellOutlineColors[i];
    delete[] cellXCoordinates[i];
    delete[] cellYCoordinates[i];
  }

  delete[] columnWidths;
  delete[] rowHeights;
  delete[] cellCenters;
  delete[] cellFillColors;
  delete[] cellOutlineColors;
  delete[] cellXCoordinates;
  delete[] cellYCoordinates;
}
