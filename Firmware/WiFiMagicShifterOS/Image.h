#define BYTESPERPIXEL 4

class MSImage {

  int width, height;
  File file;

public:
  MSImage(void)
  {
    height = width = 0;
  }

  MSImage(char *fileName)
  {
      file = SPIFFS.open(fileName, "r");
      if (file)
      {
        // TODO: real file format
        height = LEDS;
        int size = file.size();
        int frameSize = height * BYTESPERPIXEL;

        if (size % frameSize != 0)
        {
          Serial.print("File is no multiple of LED count: ");
          Serial.println(fileName);
        }
        width = size / frameSize;
      }
      else
      {
        Serial.print("File could not be opened: ");
        Serial.println(fileName);
        width = 0;
        height = LEDS;
      }
  }

  ~MSImage()
  {
    close();
  }

  int getWidth() { return width; }
  int getHeight() { return height; }

  bool readFrame(int frameIdx, byte *frameData, int maxHeight)
  {
    if (file)
    {
      file.seek(frameIdx * height * BYTESPERPIXEL, SeekSet);

      if (height < maxHeight) maxHeight = height;
      int result = file.read(frameData, maxHeight * BYTESPERPIXEL);

      if (result < maxHeight * BYTESPERPIXEL)
      {
        return false;
      }
      return true;
    }
    return false;
  }

  void close()
  {
    file.close();
  }
};
