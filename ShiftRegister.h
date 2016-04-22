#ifndef ShiftRegister_h
#define ShiftRegister_h

#include "Arduino.h"
#include <SPI.h>
#include <StandardCplusplus.h>
#include <map>

class Impl;

class ShiftRegister {
private:
  Impl *impl;
public:
  /* コンストラクタ
   * <引数>
   * ser, sck, rck :uint8_t = (SER|SRCLK|RCLK)へ接続しているArduinoのピン番号
   * byte_num :uint8_t = シフトレジスタの数(レジスタのバイト数)
   */
  ShiftRegister(uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num);
  /* USE SPI
   * Arduino	| ShiftRegister
   * ---------+--------------
   * MOSI		| SER
   * MISO		| NC
   * SCK		| SRCLK
   * SS			| RCLK
   */
  ShiftRegister(uint8_t ss, uint8_t byte_num);
  /* <引数>
   * sr :ShiftRegister = 信号を送信する*シフトレジスタのインスタンス
   * ser, sck, rck :uint8_t = (SER|SRCLK|RCLK)へ接続している*シフトレジスタのピン番号
   * byte_num :uint8_t = シフトレジスタの数
   */
  ShiftRegister(ShiftRegister *sr, uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num);

  /* レジスタの先頭へ値を追加する.
   * <説明>
   * HIGHをプッシュした場合AがHIGHになり直前のAの値はBへ記憶される,
   * これはレジスタの末端まで伝播する.
   * <引数>
   * level :(HIGH or LOW) = プッシュする値
   */
  void push(uint8_t level);
  
  /* シフトレジスタの初期値は不定なので,ユーザは必ずinit関数で初期化する必要がある. 
   * <注意>
   * レジスタを初期化する処理なので,出力は別途行う必要がある。
   * <引数>
   * value :(HIGH or LOW) = 全てのレジスタの初期値
   */
  void init(uint8_t value);
  
  /* レジスタ内の値をピンに出力する. */
  void show();
  
  /* バイト列でレジスタに値をセットする.
   * <引数>
   * data :byte[] = {'A', 'B', ... , 'H'}[]
   * siz :size_t = 配列のサイズ(プッシュするバイト数と等しい)
   */
  void set(byte data[], size_t siz);
  void set(byte data);	// 1バイト用糖衣構文

  /* 規定値の番号を別の番号へ割り当てる.
   * <注意>
   * この関数は重複した割り当てを許す.
   * <引数>
   * num :uint8_t = 割り当てる仮想ピンの番号
   * origin :uint8_t = 規定値のピン番号
   */
  void assign(uint8_t num, uint8_t origin);
  
  /* 仮想ピンへの割り当てをリセット
   * デフォルトのピン番号 0 から "シフトレジスタのビット数-1" に戻す.
   */
  void assign();
  
  /* 仮想ピンへの割り当てをクリアにし,仮想ピンを割り当てし直す.
   * <引数>
   * num :int[] = 割り当て番号の配列
   * siz :size_t = 割り当て番号の配列のサイズ数
   */
  void assign(const int num[], size_t siz);
  void assign(const int num[8]);	// 1バイト用糖衣構文

  /* 指定した番号から自動的に仮想ピンを割り当てる.
   * <引数>
   * start_num :int = 割り当てを開始する仮想ピン番号
   */
  void assign(int start_num);

  /* 指定したレジスタのみ値を変更する.
   * <引数>
   * num :uint8_t = シフトレジスタの番号
   * value :uint8_t = (HIGH or LOW)
   * <備考>
   * シフトレジスタのデフォルトの番号は [0, 1, ..., 7, ...] であり,
   * シフトレジスタのピン [A, B, ..., H, ...] に対応している.
   */
  void digitalWrite(uint8_t num, uint8_t value);

  /* 現在設定されている値を取得する.
   * <引数>
   * index :uint8_t = 先頭から数えたシフトレジスタの番号
   */
  byte getState(uint8_t index);
};

#endif