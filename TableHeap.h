//BUG: rounding errors if incorrect values are passed to setRowHeight and setColumnWidth, relative to table's size

#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

class TableHeap {
public:
	TableHeap(TFT_eSPI *tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
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

	~TableHeap();

	private:
	TFT_eSPI *tft;

	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;

	uint8_t rows, columns;

	uint16_t paddingTop, paddingBottom, paddingLeft, paddingRight;

	uint16_t *columnWidths;
	uint16_t *rowHeights;
	uint16_t ***cellCenters;
	uint16_t** cellXCoordinates;
	uint16_t** cellYCoordinates;

	uint16_t outlineColor, fillColor;
	uint16_t **cellFillColors;
	uint16_t **cellOutlineColors;

	void allocateArrays();
	void calculateColumnWidths();
	void calculateRowHeights();
	void calculateCellCenters();
	void calculateCellCoordinates();
};