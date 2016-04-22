# What is this?
This version 1.0.0 (Release: JTC 2016/03/09 17:42)  
This is the library to operate the shift register.
Using pin MOSI, MISO and SCK for [SPI](https://www.arduino.cc/en/Reference/SPI).  

| Arduino	| ShiftRegister |
| --------- | ------------- |
| MOSI		| SER			|
| MISO		| NC			|
| SCK		| SRCLK			|
| SS		| RCLK			|

This library is using library that is maniacbug's [StandardCplusplus](https://github.com/maniacbug/StandardCplusplus).

# How do I install it?
Scketch > Include Libraly > Add .ZIP Libraly... > ShiftRegister.zip  

# Dependent libraries
When use this library, you need to add the library StandardCplusplus.
After that include this library.

# What is the License?
This library is LGPL License.

# How do I use to this liblary

### ShiftRegister Class

#### ShiftRegister(uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num)
* Summary
  * Constractor
* Argments
  * ser, sck, rck :uint8_t = (SER|SRCLK|RCLK)へ接続しているArduinoのピン番号
  * byte_num :uint8_t = シフトレジスタの数(レジスタのバイト数)

***
#### ShiftRegister(uint8_t ss, uint8_t byte_num)
* Summary
  * USE SPI mode
* Argments
  * ss :uint8_t
  * byte_num :uint8_t = シフトレジスタの数(レジスタのバイト数)

***
#### ShiftRegister(ShiftRegister *sr, uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num)
* Argments
  * sr :ShiftRegister = 信号を送信する*シフトレジスタのインスタンス
  * ser, sck, rck :uint8_t = (SER|SRCLK|RCLK)へ接続している*シフトレジスタのピン番号
  * byte_num :uint8_t = シフトレジスタの数

***
#### void push(uint8_t level)
* Summary
  * レジスタの先頭へ値を追加する.
  * HIGHをプッシュした場合AがHIGHになり直前のAの値はBへ記憶される,
  * これはレジスタの末端まで伝播する.
* Argments
  * level :(HIGH or LOW) = プッシュする値

***
#### void init(uint8_t value)
* Summary
  * シフトレジスタの初期値は不定なので,ユーザは必ずinit関数で初期化する必要がある. 
* Warning
  * レジスタを初期化する処理なので,出力は別途行う必要がある。
* Argments
  * value :(HIGH or LOW) = 全てのレジスタの初期値

***
#### void show()
* Summary
  * レジスタ内の値をピンに出力する.

***
#### void set(byte data[], size_t siz)
#### void set(byte data) ``// 1バイト用``
* Summary
  * バイト列でレジスタに値をセットする.
* Argments
  * data :byte[] = {'A', 'B', ... , 'H'}[]
  * siz :size_t = 配列のサイズ(プッシュするバイト数と等しい)

***
#### void assign(uint8_t num, uint8_t origin)
* Summary
  * 規定値の番号を別の番号へ割り当てる.
* Warning
  * この関数は重複した割り当てを許す.
* Argments
  * num :uint8_t = 割り当てる仮想ピンの番号
  * origin :uint8_t = 規定値のピン番号

***
#### void assign()
* Summary
  * 仮想ピンへの割り当てをリセット
  * デフォルトのピン番号 0 から "シフトレジスタのビット数-1" に戻す.

***
#### void assign(const int num[], size_t siz)
#### void assign(const int num[8])	``// 1バイト用``
* Summary
  * 仮想ピンへの割り当てをクリアにし,仮想ピンを割り当てし直す.
* Argments
  * num :int[] = 割り当て番号の配列
  * siz :size_t = 割り当て番号の配列のサイズ数

***
#### void assign(int start_num)
* Summary
  * 指定した番号から自動的に仮想ピンを割り当てる.
* Argments
  * start_num :int = 割り当てを開始する仮想ピン番号

***
#### void digitalWrite(uint8_t num, uint8_t value)
* Summary
  * 指定したレジスタのみ値を変更する.
* Argments
  * num :uint8_t = シフトレジスタの番号
  * value :uint8_t = (HIGH or LOW)
* Explanation
  * シフトレジスタのデフォルトの番号は [0, 1, ..., 7, ...] であり,
  * シフトレジスタのピン [A, B, ..., H, ...] に対応している.

***
#### byte getState(uint8_t index)
* Summary
  * 現在設定されている値を取得する.
* Argments
  * index :uint8_t = 先頭から数えたシフトレジスタの番号
