/**
    The goal (and current functionality) of this library is to allow the developer to programmatically
    create music and sound

    In the very near future this will be transformed into a realtime threaded class which will allow you
    to mix sounds on the go, and will perform all audio functions via callback, but for now everything is
    transcribed into notes (which are on the modern scale) as the number of half steps from middle C

    Sounds can be played simultaneously by using the mix flag in the note struct
**/

#include <windows.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include "ULTI_File.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif // M_PI

namespace ulti{

    //make a double word big endian, this will have no effect on a big endian system
    DWORD toBig(DWORD in);

    //make a double word little endian, this will have no effect on a little endian system (like x86)
    DWORD toLittle(DWORD in);

    //As defined by the standard, see here http://soundfile.sapp.org/doc/WaveFormat/
    struct WAVHeader{
        union{
            struct{
                DWORD chunkID;       // 0x52494646 "RIFF" in big endian
                DWORD chunkSize;     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
                DWORD format;        // 0x57415645 "WAVE" in big endian

                DWORD subChunk1ID;   // 0x666d7420 "fmt " in big endian
                DWORD subChunk1Size; // 16 for PCM
                WORD  audioFormat;   // 1 for PCM
                WORD  numChannels;   // 1 for mono, 2 for stereo
                DWORD sampleRate;    // 8000, 22050, 44100, etc...
                DWORD byteRate;      // sampleRate * numChannels * bitsPerSample/8
                WORD  blockAlign;    // numChannels * bitsPerSample/8
                WORD  bitsPerSample; // number of bits (8 for 8 bits, etc...)

                DWORD subChunk2ID;   // 0x64617461 "data" in big endian
                DWORD subChunk2Size; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)
                union{
                    unsigned char* data;
                    unsigned int * idata;
                    unsigned long * ldata;
                    unsigned short * sdata;
                };
            };
            unsigned char raw[];
        };
    };

    struct Wav{
        char* data;
        char* wavpoint;
        int wavsize;
        void setupHeader(DWORD NumSamples = 1, WORD BitsPerSample = 16, WORD NumChannels= 1,  DWORD SampleRate = 44100);
        Wav();
        Wav(const Wav& in);
        Wav& operator=(const Wav& in);
        ~Wav();
        void resize(int nsize);
    };

    enum notestyle{
        noteSine,
        noteSquare,
        noteSaw,
        noteSilence,
        noteStatic
    };

    //   #       #       |       #       #       |       #       #       #       |       #
    //   #       #       |       #       #       |       #       #       #       |       #
    //   |   a   |   b   |   c   |   d   |   e   |   f   |   g   |   a   |   b   |   c   |
    //  -4  -3  -2  -1       0   1   2   3   4       5   6   7   8   9   10  11      12  13


    enum notestep{
        NOTE_A = -3,
        NOTE_ASHARP = -2,
        NOTE_BFLAT = NOTE_ASHARP,
        NOTE_B = -1,
        NOTE_C = 0,
        NOTE_CSHARP = 1,
        NOTE_DFLAT = NOTE_CSHARP,
        NOTE_D = 2,
        NOTE_DSHARP = 3,
        NOTE_EFLAT = NOTE_DSHARP,
        NOTE_E = 4,
        NOTE_F = 5,
        NOTE_FSHARP = 6,
        NOTE_GFLAT = NOTE_FSHARP,
        NOTE_G = 7,
        NOTE_GSHARP = 8,
        NOTE_AFLAT = NOTE_GSHARP
    };

    struct note{
        double hz;
        int scale;
        int key;

        int duration;
        int position;//starting ms
        int branch;
        notestyle style;
        bool mix;
        int channel;
        note(double ihz,int ims, notestyle istyle=noteSine, bool imix = false);
        note(double ihz, int iposition, int ibranch, int iduration, notestyle istyle=noteSine);
        note(int iscale, int ikey, int iposition, int ibranch, int iduration, notestyle istyle=noteSine);
    };



    struct song{
        std::vector<note> notes;

    };


    const static double twelvethroot = 1.059463094359;
    double hzforfreq(int halfstepsfromA3);
    std::string getletterforoffstep(int offstep);

    void playSong(std::vector<note> song, int volume, bool repeat = false);
    void playSongAsync(std::vector<note> song, int volume, bool repeat = false);
    void saveSong(std::vector<note> song, int volume, const std::string& filename);

    void playSine(double hz, int ms, double volume = 32768);
    void playSquare(int hz, int ms);
    void playStatic(int hz, int ms);
    void playSaw(int hz, int ms);
    void stopSound();
    void playWav(WAVHeader& wav);

    class soundProvider{
    public:

    };

};
