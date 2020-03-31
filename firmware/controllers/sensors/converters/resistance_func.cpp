/**
 * @author Matthew Kennedy, (c) 2019
 */

#include "resistance_func.h"
#include "loggingcentral.h"

void ResistanceFunc::configure(float supplyVoltage, float pullupResistor) {
	m_pullupResistor = pullupResistor;
	m_supplyVoltage = supplyVoltage;
}

SensorResult ResistanceFunc::convert(float raw) const {
	// If the voltage is very low, the sensor is a dead short.
	if (raw < 0.05f) {
		return {false, 0.0f};
	}

	// If the voltage is very high (95% VCC), the sensor is open circuit.
	if (raw > (m_supplyVoltage * 0.95f)) {
		return {false, 1e6};
	}

	// Voltage is in a sensible range - convert
	float resistance = m_pullupResistor / (m_supplyVoltage / raw - 1);

	return {true, resistance};
}

void ResistanceFunc::showInfo(Logging* logger, float testInputValue) const {
	const auto [valid, value] = convert(testInputValue);
	scheduleMsg(logger, "    %.2f volts -> %.1f ohms, with supply voltage %.2f and pullup %.1f.", testInputValue, value, m_supplyVoltage, m_pullupResistor);
}
