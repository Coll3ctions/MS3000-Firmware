class MagicBeatMode : public MagicShifterBaseMode {

private:

	int frame = 0;
	long avgZ = 0;
	long xPos;

	int colIdx = 1;
	int discoIdx = 0;
	uint8_t r, g, b;


public:
	MagicBeatMode() {
		modeName = "Beat";
	}

	virtual void start() {
			r = g = 0;
			b = 255;
	}

	virtual void stop(void) {
	}

#define FIXED_SHIFT 8

	virtual bool step(void) {

		avgZ *= 31;
		avgZ += msGlobals.ggAccelCount[2];
		if (avgZ >= 0) {
			avgZ += 0 + 15;
		} else {
			avgZ -= 0 + 15;
		}
		avgZ /= 32;

		frame++;

		if (!discoIdx) {
			xPos = 7 * (1 << FIXED_SHIFT) + (1 << (FIXED_SHIFT - 1)) + (msGlobals.ggAccelCount[2] - avgZ) * 40;
			if (xPos < 0)
				xPos = 0;
			if (xPos > 15 * (1 << FIXED_SHIFT))
				xPos = 15 * (1 << FIXED_SHIFT);

			int8_t xPosInt = xPos >> FIXED_SHIFT;			
			uint8_t xPosRemainder = (xPos & (1 << FIXED_SHIFT) - 1);

			msSystem.msLEDs.fillLEDs(0, 0, 0);
			
			msSystem.msLEDs.setLED(xPosInt + 1, (r * xPosRemainder) >> FIXED_SHIFT, (g * xPosRemainder) >> FIXED_SHIFT, (b * xPosRemainder) >> FIXED_SHIFT,
				msGlobals.ggBrightness);

			xPosRemainder = ((1 << FIXED_SHIFT) - 1) - xPosRemainder;			

			msSystem.msLEDs.setLED(xPosInt, (r * xPosRemainder) >> FIXED_SHIFT, (g * xPosRemainder) >> FIXED_SHIFT, (b * xPosRemainder) >> FIXED_SHIFT,
				msGlobals.ggBrightness);

			msSystem.msLEDs.updateLEDs();

		} else {
			xPos = (msGlobals.ggAccelCount[2] - avgZ);
			if (xPos < 0)
				xPos = -xPos;
			xPos >>= (discoIdx - 1);
			xPos -= 5;
			if (xPos > 16)
				xPos = 16;

			msSystem.msLEDs.fillLEDs(0, 0, 0);

			for (int i = 0; i < xPos; i++)
				msSystem.msLEDs.setLED(15 - i, r, g, b, msGlobals.ggBrightness);

			msSystem.msLEDs.updateLEDs();
		}

		if (msSystem.msButtons.msBtnAHit)
		{
			discoIdx = (discoIdx + 1) % 6;
			msSystem.msButtons.msBtnAHit = false;
		}

		if (msSystem.msButtons.msBtnBHit)
		{
			discoIdx = (discoIdx + 5) % 6;
			msSystem.msButtons.msBtnBHit = false;
		}

		if (msSystem.msButtons.msBtnPwrHit) {
			colIdx++;
			if (colIdx > 7) colIdx = 1;
			r = (colIdx & 1) ? 255 : 0;
			g = (colIdx & 2) ? 255 : 0;
			b = (colIdx & 4) ? 255 : 0;
			msSystem.msButtons.msBtnPwrHit = false;
		}

	} 
};