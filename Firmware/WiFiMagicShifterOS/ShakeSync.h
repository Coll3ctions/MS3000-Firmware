#define PREDICT_SAME_DIRECTION true

class POVShakeSync {

  typedef struct struct_ShakePoint {
    float g;
    int micros; // absolute
  } ShakePoint;

  const float hysteresis = 0.4;
	const float sensitivity = 0.4;

	// last time it took to go from acceleration minimum to maximum/ from max to min
	int min2maxDelta, max2minDelta;
	int min2maxFrames, max2minFrames;

	// we use these to track the timestamp of the current min resp. max, might change since we are not sure we have found it yet
  ShakePoint activeMin, activeMax;
  int activeFramesMin2Max, activeFramesMax2Min;
  // last detected timestamps of the last min resp. max,
  ShakePoint lastMin, lastMax;
  int press = 0;

  // at each moment we might be either searching for the next min (then searchMin == true) or max then searchMin == false
  bool searchMin = false;

  // used to make sure the events only fire once the first time their condition is met
  bool firedPredictedZero = false;
  bool firedPredictedExtremum = false;

	int sameCount = 0, diffCount = 0;

	int frames = 16;

  int frameStartTime;

  int frameIndex = 0;
  bool isActive = false; // is shaking happening at all?
  // are we in a shaking position that has a frame to display? Is frameIndex valid?
  bool isFrameIndexActive = false;

public:
  // sensitivity is the min distance between min and max to start poving
  POVShakeSync(void)
  {
  }

	void setFrames(int w)
	{
		frames = w;
	}

	int getFrameIndex()
	{
    if (!digitalRead(PIN_BUTTON1) && searchMin)
    {
      return -1;
    }
    if (isActive && isFrameIndexActive)
  	{
      return frameIndex;
    }
    else
    {
      return -1;
    }
	}

	// returns true if POV shake is actve
  bool update(float g)
  {
    int currentMicros = micros();
		ShakePoint currentPoint;

    // frame handling
  	if (isActive && isFrameIndexActive)
  	{
  		frameIndex += searchMin ? -1 : 1;
  		// under or overflow
  		if (frameIndex < 0 || frameIndex >= frames)
      {
  			isFrameIndexActive = false;
      }
  	}
  	else
  	{
  		// TODO: make values configurable
  		if (lastMin.micros > currentMicros + 500 * 1000 || lastMin.g + 1. > lastMax.g)
        isActive = false;
      else
        isActive = true;
    }



		fillPixels(0, 0, 0);

    activeFramesMin2Max++;
    activeFramesMax2Min++;

		if (searchMin)
		{
			if (g < activeMin.g)
			{
			  activeMin.g = g;
			  activeMin.micros = currentMicros;
        activeFramesMin2Max = 0;
			}
      else
      {
        // TODO: timeout
      }

      // could be timeout of condition above (g < activeMin.g) then we dont need hysteresis
			if (g > (activeMin.g + hysteresis))
			{
				int lastV = max2minDelta;

        max2minFrames = activeFramesMax2Min - activeFramesMin2Max;

				lastMin = activeMin;
				max2minDelta = lastMin.micros - lastMax.micros;
				searchMin = false;
        firedPredictedZero = false;
        firedPredictedExtremum = false;

        frameStartTime = lastMin.micros + min2maxDelta/2 - ((((float)min2maxFrames) / min2maxDelta) * frames) / 2;
				// end previous frame since it wont fit anyway (we are allready moving in other dir)
        isFrameIndexActive = false;

				int dSame = max2minDelta - lastV;
				if (dSame < 0) dSame = -dSame;
				int dDiff = max2minDelta - min2maxDelta;
				if (dDiff < 0) dDiff = -dDiff;
				if (dSame <= dDiff)
				{
					Serial.println("same");
					sameCount++;
				}
				else
				{
					Serial.println("diff");
					diffCount++;
				}
				Serial.println(sameCount-diffCount);
				//Serial.println(dDiff);
				//Serial.println(activeMin.g);

				// reset max
				activeMax.g = g;
				activeMax.micros = currentMicros;

				//fillPixels(3, 0, 3);
			}
			// last this dir or last other dir
			//else if (!firedPredictedZero && (currentMicros > lastMax.micros + min2maxDelta/2)) // ikes
      //else if (!firedPredictedZero && (currentMicros > lastMax.micros + max2minDelta/2)) // correct
      else if (!firedPredictedZero && currentMicros > frameStartTime)
			{
        frameIndex = frames - 1;
        isFrameIndexActive = true;
			  firedPredictedZero = true;

			  //fillPixels(5, 0, 0);
			}
			else if (!firedPredictedExtremum && currentMicros >= lastMax.micros + max2minDelta)
			{
				firedPredictedExtremum = true;
				//fillPixels(20, 0, 0);
			}
		}
		else {
			if (g > activeMax.g)
			{
			  activeMax.g = g;
			  activeMax.micros = currentMicros;
				activeFramesMax2Min = 0;
			}
			else
			{
			}

			if (g < (activeMax.g - hysteresis))
			{
        min2maxFrames = activeFramesMin2Max - activeFramesMax2Min;
				lastMax = activeMax;
				min2maxDelta = lastMax.micros - lastMin.micros;
				searchMin = true;
				firedPredictedZero = false;
				firedPredictedExtremum = false;

        frameStartTime = lastMin.micros + max2minDelta/2 - ((((float)max2minFrames) / max2minDelta) * frames) / 2;
				// end previous frame since it wont fit anyway (we are allready moving in other dir)
        isFrameIndexActive = false;

				// reset min
				activeMin.g = g;
				activeMin.micros = currentMicros;

				//fillPixels(0, 3, 3);
			}
			//else if (!firedPredictedZero && (currentMicros > lastMin.micros + max2minDelta/2)) // ikes
			//else if (!firedPredictedZero && (currentMicros > lastMin.micros + min2maxDelta/2)) // ok
			else if (!firedPredictedZero && currentMicros > frameStartTime)
      {
        frameIndex = 0;
        isFrameIndexActive = true;
			  firedPredictedZero = true;
			  //fillPixels(0, 5, 0);
			}
		}

		updatePixels();

    return isActive;
	}
};
