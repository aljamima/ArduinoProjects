/*
  SimpleBME280.h
  Created by RIVA, 2021.

  Простой драйвер для метеодатчика Bosch Sensortec BME280 I2C.
  Используется стандартная библиотека I2C Wire.

  2021.06.03 - v1.0 (первый выпуск).
*/

#ifndef SIMPLE_BME280_H
#define SIMPLE_BME280_H

/*******************************************************************************/

#include <stdint.h>

/**************************** BME280 Definitions *******************************/

#define BME280_LIB_VERSION              0x10    // версия библиотеки 0xAB->vA.B
#define BME280_ADDRESS                  0x76    // адрес BME280 на шине I2C
#define BME280_CHIP_ID                  0x60    // ID BME280

/***************************** BME280 Registers ********************************/

#define BME280_REG_DIG_T1               0x88
#define BME280_REG_DIG_T2               0x8A
#define BME280_REG_DIG_T3               0x8C

#define BME280_REG_DIG_P1               0x8E
#define BME280_REG_DIG_P2               0x90
#define BME280_REG_DIG_P3               0x92
#define BME280_REG_DIG_P4               0x94
#define BME280_REG_DIG_P5               0x96
#define BME280_REG_DIG_P6               0x98
#define BME280_REG_DIG_P7               0x9A
#define BME280_REG_DIG_P8               0x9C
#define BME280_REG_DIG_P9               0x9E

#define BME280_REG_DIG_H1               0xA1
#define BME280_REG_DIG_H2               0xE1
#define BME280_REG_DIG_H3               0xE3
#define BME280_REG_DIG_H4               0xE4
#define BME280_REG_DIG_H5               0xE5
#define BME280_REG_DIG_H6               0xE7

#define BME280_REG_CHIPID               0xD0
#define BME280_REG_VERSION              0xD1
#define BME280_REG_RESET                0xE0

#define BME280_REG_CTRLHUM              0xF2
#define BME280_REG_STATUS               0xF3
#define BME280_REG_CONTROL              0xF4
#define BME280_REG_CONFIG               0xF5
#define BME280_REG_PDATA                0xF7
#define BME280_REG_TDATA                0xFA
#define BME280_REG_HDATA                0xFD

/********************** BME280 Register bitfield positions *********************/

#define BME280_BITMASK_HOS_SHIFT        0
#define BME280_BITMASK_POS_SHIFT        2
#define BME280_BITMASK_TOS_SHIFT        5
#define BME280_BITMASK_FILTER_SHIFT     2
#define BME280_BITMASK_MODE_SHIFT       0
#define BME280_BITMASK_FILTER_SHIFT     2
#define BME280_BITMASK_PERIOD_SHIFT     5
#define BME280_BITMASK_MEASURING_SHIFT  3
#define BME280_BITMASK_IMUPDATE_SHIFT   0

/*********************** BME280 Register bitfield masks ************************/

#define BME280_BITMASK_HOS              (0x07 << BME280_BITMASK_HOS_SHIFT)
#define BME280_BITMASK_POS              (0x07 << BME280_BITMASK_POS_SHIFT)
#define BME280_BITMASK_TOS              (0x07 << BME280_BITMASK_TOS_SHIFT)
#define BME280_BITMASK_MODE             (0x03 << BME280_BITMASK_MODE_SHIFT)
#define BME280_BITMASK_FILTER           (0x07 << BME280_BITMASK_FILTER_SHIFT)
#define BME280_BITMASK_PERIOD           (0x07 << BME280_BITMASK_PERIOD_SHIFT)
#define BME280_BITMASK_MEASURING        (0x01 << BME280_BITMASK_MEASURING_SHIFT)
#define BME280_BITMASK_IMUPDATE         (0x01 << BME280_BITMASK_IMUPDATE_SHIFT)
#define BME280_BITMASK_RESET            0xB6

/*******************************************************************************/
/*******************************************************************************/


/********************* Используемые библиотекой типы данных ********************/

typedef enum {      // Коэффициент передискретизации:
  bmeoSkip = 0,     // канал измерения выключен (считывается 0x800000)
  bmeo1,            // x1
  bmeo2,            // x2
  bmeo4,            // x4
  bmeo8,            // x8
  bmeo16            // x16
} BME280oversampling_t;

typedef enum {      // Режим работы:
  bmemSleep = 0,    // ожидание (сон)
  bmemForced,       // однократный запуск (после чего - возврат к ожиданию)
  bmemNormal = 3    // нормальный (циклический автоматический запуск измерения)
} BME280mode_t;

typedef enum {      // Коэффициент рекурсивного фильтра (IIR filter):
  bmefOff = 0,      // фильтр выключен
  bmef2,            // 2
  bmef4,            // 4
  bmef8,            // 8
  bmef16            // 16
} BME280filter_t;

typedef enum {      // Период неактивности датчика (между циклами измерениями):
  bmep0_5ms = 0,    // 0,5 мс
  bmep62_5ms,       // 62,5 мс
  bmep125ms,        // 125 мс
  bmep250ms,        // 250 мс
  bmep500ms,        // 500 мс
  bmep1s,           // 1 с
  bmep10ms,         // 10 мс
  bmep20ms          // 20 мс
} BME280period_t;

/***************** Значения регистров настройки при инициализации **************/

#define BME280_DEFAULT_HOS      (bmeo4      << BME280_BITMASK_HOS_SHIFT)
#define BME280_DEFAULT_TOS      (bmeo2      << BME280_BITMASK_TOS_SHIFT)
#define BME280_DEFAULT_POS      (bmeo16     << BME280_BITMASK_POS_SHIFT)
#define BME280_DEFAULT_MODE     (bmemNormal << BME280_BITMASK_MODE_SHIFT)
#define BME280_DEFAULT_PERIOD   (bmep250ms  << BME280_BITMASK_PERIOD_SHIFT)
#define BME280_DEFAULT_FILTER   (bmef8      << BME280_BITMASK_FILTER_SHIFT)

/*******************************************************************************/

class SimpleBME280 {

  public:

    uint8_t     begin();                    // инициализация датчика и библиотеки
    void        reset();                    // сброс

    // общие установки
    void        mode(BME280mode_t x);       // установка режима работы
    void        period(BME280period_t x);   // установка периода неактивности (больше период - меньше частота замеров)
    void        filter(BME280filter_t x);   // установка коэффициента фильтрации

    // установка коэффициента передискретизации (к.п.) канала измерения
    void        oversampling(BME280oversampling_t x, char channel); // установка к.п. канала (канал: t/p/h)
    void        oversamplingT(BME280oversampling_t x);              // установка к.п. канала темп-ры
    void        oversamplingP(BME280oversampling_t x);              // установка к.п. канала давления
    void        oversamplingH(BME280oversampling_t x);              // установка к.п. канала влажности

    // работа с датчиком
    void        update();   // считывание и обработка данных датчика
    float       getT();     // [гр.C] температура
    uint32_t    getP();     // [ Па ] давление абсолютное
    float       getH();     // [ %  ] влажность относительная

  private:

    int32_t     t_fine, x, y, r, p, h;
    uint8_t     h1, h3;
    int8_t      h6;
    uint16_t    t1, p1;
    int16_t     t2, t3, p2, p3, p4, p5, p6, p7, p8, p9, h2, h4, h5;

    uint8_t     read8(uint8_t reg);
    uint16_t    read16(uint8_t reg);
    uint16_t    read16BE(uint8_t reg);
    uint32_t    read24(uint8_t reg);
    void        readStart(uint8_t reg, uint8_t size);
    void        send(uint8_t reg, uint8_t value);
    void        write(uint8_t reg, uint8_t value);
    void        rmw(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t value);
};


#endif /* SIMPLE_BME280_H */
