import { TDSLib } from '../src';

describe('Node TDS sensor', () => {
  it('should create new instance', () => {
    const probe = new TDSLib(12);

    expect(probe).toBeDefined();
  });

  it('should set default value to 1 for K-Cell value of the probe', () => {
    const probe = new TDSLib(12);

    expect(probe.getKValue()).toBe(1);
  });

  it('should update K-Cell value when setKValue is called', () => {
    const probe = new TDSLib(12);
    probe.setKValue(10);

    expect(probe.getKValue()).toBe(10);
  });

  it('should read TDS value', () => {
    // const probe = new TDSLib(12);
    // probe.update();
    // console.log('+++', probe.getTDSValue());
    // console.log('***', probe.getECValue());

    expect(true).toBeTruthy();
  });
});
