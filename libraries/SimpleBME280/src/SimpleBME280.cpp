#include "SimpleBME280.h"
#include <Wire.h>

#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

/****************************** Public Section *********************************/

uint8_t SimpleBME280::begin() {
  Wire.begin();
  delay(2);

  if (read8(BME280_REG_CHIPID) != BME280_CHIP_ID)
    return 1;

  reset();

  t1 = (uint16_t)read16BE(BME280_REG_DIG_T1);
  t2 = (int16_t)read16BE(BME280_REG_DIG_T2);
  t3 = (int16_t)read16BE(BME280_REG_DIG_T3);

  p1 = (uint16_t)read16BE(BME280_REG_DIG_P1);
  p2 = (int16_t)read16BE(BME280_REG_DIG_P2);
  p3 = (int16_t)read16BE(BME280_REG_DIG_P3);
  p4 = (int16_t)read16BE(BME280_REG_DIG_P4);
  p5 = (int16_t)read16BE(BME280_REG_DIG_P5);
  p6 = (int16_t)read16BE(BME280_REG_DIG_P6);
  p7 = (int16_t)read16BE(BME280_REG_DIG_P7);
  p8 = (int16_t)read16BE(BME280_REG_DIG_P8);
  p9 = (int16_t)read16BE(BME280_REG_DIG_P9);

  h1 = (uint8_t  )read8(BME280_REG_DIG_H1);
  h2 = (int16_t  )read16BE(BME280_REG_DIG_H2);
  h3 = (uint8_t  )read8(BME280_REG_DIG_H3);
  h4 = ((uint16_t)read8(BME280_REG_DIG_H4 + 0) << 8) + (uint8_t)(read8(BME280_REG_DIG_H4 + 1) << 4);
  h5 = ((uint16_t)read8(BME280_REG_DIG_H5 + 1) << 8) + (uint8_t)(read8(BME280_REG_DIG_H5 + 0) << 0);
  h6 = (int8_t   )read8(BME280_REG_DIG_H6);
  h4 >>= 4;
  h5 >>= 4;

  write(BME280_REG_CTRLHUM, BME280_DEFAULT_HOS );
  write(BME280_REG_CONTROL, BME280_DEFAULT_POS    | BME280_DEFAULT_TOS | BME280_DEFAULT_MODE );
  write(BME280_REG_CONFIG,  BME280_DEFAULT_PERIOD | BME280_DEFAULT_FILTER );
  return 0;
}

void SimpleBME280::reset() {
  send(BME280_REG_RESET, BME280_BITMASK_RESET);
  delay(2);
}

void SimpleBME280::mode(BME280mode_t x) {
  rmw(BME280_REG_CONTROL, BME280_BITMASK_MODE, BME280_BITMASK_MODE_SHIFT, (uint8_t)x);
}

void SimpleBME280::period(BME280period_t x) {
  rmw(BME280_REG_CONFIG, BME280_BITMASK_PERIOD, BME280_BITMASK_PERIOD_SHIFT, (uint8_t)x);
}

void SimpleBME280::filter(BME280filter_t x) {
  rmw(BME280_REG_CONFIG, BME280_BITMASK_FILTER, BME280_BITMASK_FILTER_SHIFT, (uint8_t)x);
}

void SimpleBME280::oversampling(BME280oversampling_t x, char channel) {
  channel &= ~0x20;

  if (channel == 'T')
    rmw(BME280_REG_CONTROL, BME280_BITMASK_TOS, BME280_BITMASK_TOS_SHIFT, (uint8_t)x);

  if (channel == 'P')
    rmw(BME280_REG_CONTROL, BME280_BITMASK_POS, BME280_BITMASK_POS_SHIFT, (uint8_t)x);

  if (channel == 'H')
    rmw(BME280_REG_CTRLHUM, BME280_BITMASK_HOS, BME280_BITMASK_HOS_SHIFT, (uint8_t)x);
}

void SimpleBME280::oversamplingT(BME280oversampling_t x) {
  rmw(BME280_REG_CONTROL, BME280_BITMASK_TOS, BME280_BITMASK_TOS_SHIFT, (uint8_t)x);
}

void SimpleBME280::oversamplingP(BME280oversampling_t x) {
  rmw(BME280_REG_CONTROL, BME280_BITMASK_POS, BME280_BITMASK_POS_SHIFT, (uint8_t)x);
}

void SimpleBME280::oversamplingH(BME280oversampling_t x) {
  rmw(BME280_REG_CTRLHUM, BME280_BITMASK_HOS, BME280_BITMASK_HOS_SHIFT, (uint8_t)x);
}


void SimpleBME280::update() {
  // ================================ Температура ===================================
  x = read24(BME280_REG_TDATA);
  x >>= 4;  x -= t1;
  t_fine  = t3;  t_fine *= x;  t_fine >>= 16;
  t_fine += t2;  t_fine *= x;  t_fine >>= 10;
  // ================================================================================

  // ================================== Давление ====================================
  x = t_fine; x -= 128000L;

  y = r = x;  y *= x;  y >>= 16;  y *= p6;  y >>= 1;
  r *= p5;    y += r;  y >>= 3;   r  = p4;  r <<= 15;  y += r;

  r = 1L << 20;
  r -= read24(BME280_REG_PDATA); //  r -= 306093;
  r <<= 11;  r -= y;

  y = x;   y *= p3;  y >>= 8;  y *= x;  y >>= 12;
  x *= p2; y += x;   y >>= 8;  y += 1L << 27;
  y = ((uint64_t)y * p1) >> 19; //  y >>= 12; y *= p1; y >>= 7;

  r = (uint64_t)r * 3125 / y; //  r *= 3125;  r /= y;
  p = x = r;  p *= p9;  p >>= 18;  p *= r;  x  *= p8;  p += x;
  p >>= 8;    x  = p7;  x <<= 9;   p += x;  r <<= 11;  p += r;  p >>= 13;
  // ================================================================================

  // ================================== Влажность ===================================
  x = t_fine;  x -= 76800L;

  r = y = x;  r *= h3;  r += 1L << 26;  r >>= 16;  y *= h6;
  y >>= 16;   r *= y;   r += 1L << 20;  r >>= 12;  r *= h2;  r >>= 8;

  x *= h5;  y = -(uint32_t)h4;  y <<= 6;
  y += read16(BME280_REG_HDATA); //  y += 22123;
  y <<= 14;   y -= x;   y >>= 14;       r *= y;    r >>= 8;

  h = r;     r *= h1;   r = -r;   r += 1L << 27;   r >>= 16;  h *= r;
  // ================================================================================
}

float SimpleBME280::getT() {
  return (float)t_fine / 5120.0;
}

uint32_t SimpleBME280::getP() {
  return p;
}

float SimpleBME280::getH() {
  if (h < 0             ) return 0;
  if (h > (100UL << 19) ) return 100.0;
  return (float)h / float(1UL << 19);
}


/********************************* Private Section *****************************/

uint8_t SimpleBME280::read8(uint8_t reg) {
  readStart(reg, 1);
  return Wire.read();
}

uint16_t SimpleBME280::read16(uint8_t reg) {
  readStart(reg, 2);
  uint16_t data;
  data   = (uint8_t)Wire.read();
  data <<= 8;
  data  |= (uint8_t)Wire.read();
  return data;
}

uint16_t SimpleBME280::read16BE(uint8_t reg) {
  uint16_t data = read16(reg);
  return (data << 8) | (data >> 8);
}

uint32_t SimpleBME280::read24(uint8_t reg) {
  readStart(reg, 3);
  uint32_t data;
  data   = (uint8_t)Wire.read();
  data <<= 8;
  data  |= (uint8_t)Wire.read();
  data <<= 8;
  data  |= (uint8_t)Wire.read();
  return data >> 4;
}

void SimpleBME280::readStart(uint8_t reg, uint8_t size) {
  Wire.beginTransmission(BME280_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(BME280_ADDRESS, size);
  while (Wire.available() < size);
}

void SimpleBME280::send(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(BME280_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void SimpleBME280::write(uint8_t reg, uint8_t value) {
  do send(reg, value);
  while (value != read8(reg));
}

void SimpleBME280::rmw(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t value) {
  uint8_t data = read8(reg);

  uint8_t tmp;
  if (reg != BME280_REG_CONTROL)
    tmp = read8(BME280_REG_CONTROL);
  write(BME280_REG_CONTROL, 0);

  value <<=  shift;
  value  &=  mask;
  data   &= ~mask;
  data   |=  value;
  write(reg, data);

  if (reg != BME280_REG_CONTROL)
    write(BME280_REG_CONTROL, tmp);
}
