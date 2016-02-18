
#define FRAME_MULTIPLY 2

class MagicPOVMode:public MagicShifterBaseMode {

  private:
	MagicShifterImageAbstr * msImage = NULL;
	POVShakeSync shakeSync;
	bool correctBrightness = false;

  public:

	void setImage(MagicShifterImageAbstr * lImage) {
		// if (msImage != NULL) 
		//   msImage->close();

		msImage = lImage;

		if (lImage != NULL) {
			shakeSync.setFrames(msImage->getWidth() * FRAME_MULTIPLY);
		} else
			 shakeSync.setFrames(0);
	}

	void start() {
		// setImage(NULL);
	}

// stop the mode
	void stop(void) {
		shakeSync.setFrames(0);
	}

// step through a frame of the mode 
	bool step() {
		if (msImage != NULL) {
			// check accelerometer
			if (shakeSync.update(msGlobals.ggAccel[YAXIS])) {
				int index = shakeSync.getFrameIndex();

				if (index > 0) {
					byte povData[RGB_BUFFER_SIZE];

					int frame_index = index / FRAME_MULTIPLY;

					msImage->getFrameData(frame_index, povData);

					if (correctBrightness) {

						msSystem.msLEDs.loadBufferShort(povData);
						msSystem.msLEDs.updateLEDs();
						//delayMicroseconds(POV_TIME_MICROSECONDS);
						msSystem.msLEDs.loadBufferLong(povData);
						msSystem.msLEDs.updateLEDs();
					} else {
						msSystem.msLEDs.loadBuffer(povData);
						msSystem.msLEDs.updateLEDs();
						delayMicroseconds(msGlobals.ggBrightness * 16);

						if (msGlobals.ggBrightness < 31)
							msSystem.msLEDs.fastClear();
					}
				} else {
					msSystem.msLEDs.fastClear();
					yield();
				}
				return true;
			} else
				return false;
		}
		return false;
	}
};
