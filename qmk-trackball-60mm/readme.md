# Frivermen Trackball

Самодельный трекбол на RP2040 с сенсором PAW3204 и 4 кнопками.

* Keyboard Maintainer: [frivermen](https://github.com/frivermen)
* Hardware Supported: RP2040, PAW3204 sensor
* Hardware Availability: DIY

## Сборка

    qmk compile -kb frivermen/trackball -km default

## Подключение

| Компонент      | Пин RP2040 |
|----------------|------------|
| PAW3204 SCK    | GP6        |
| PAW3204 SDIO   | GP7        |
| Кнопка 1 (ЛКМ) | GP16       |
| Кнопка 2 (СКМ) | GP15       |
| Кнопка 3 (ПКМ) | GP14       |
| Кнопка 4 (Скролл) | GP17   |
