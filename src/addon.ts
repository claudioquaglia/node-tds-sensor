export declare class TDSLib {
  constructor(pinNumber: number);
  public read(input: number): number;
  public setTemperature(temperature: number): void;
  public setKValue(kValue: number): void;
  /**
   *
   * @param i2cValue Hex board address
   */
  public setI2CSlave(i2cValue: number): void;
  public getKValue(): number;
  public getECValue(): number;
  public getTDSValue(): number;
  public getTemperature(): number;
}

const addon = require('../build/Release/addon');
module.exports = addon;
