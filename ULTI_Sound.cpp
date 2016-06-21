#include "ULTI_Sound.h"
#include "ULTI_Thread.h"
namespace ulti{

    enum{
        O32_LITTLE_ENDIAN = 0x03020100ul,
        O32_BIG_ENDIAN = 0x00010203ul,
        O32_PDP_ENDIAN = 0x01000302ul
    };

    static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order = { { 0, 1, 2, 3 } };

    #define O32_HOST_ORDER (o32_host_order.value)

    DWORD toBig(DWORD in){
        if (O32_HOST_ORDER==O32_BIG_ENDIAN)
            return in;
        return ((in&0xff000000)>>24)|((in&0x000000ff)<<24)|((in&0x00ff0000)>>8)|((in&0x0000ff00)<<8);
    }
    DWORD toLittle(DWORD in){
        if (O32_HOST_ORDER==O32_LITTLE_ENDIAN)
            return in;
        return ((in&0xff000000)>>24)|((in&0x000000ff)<<24)|((in&0x00ff0000)>>8)|((in&0x0000ff00)<<8);
    }
    Wav::Wav(){
        data=NULL;
        wavpoint=NULL;
    };
    void Wav::setupHeader(DWORD NumSamples, WORD BitsPerSample, WORD NumChannels,  DWORD SampleRate){

        if (data!=NULL)
            delete[] (data);
        wavsize=sizeof(WAVHeader)+NumChannels*NumSamples*BitsPerSample/8;
        data=new char[wavsize];

        WAVHeader* tmp = (WAVHeader*)data;
        wavpoint=&data[sizeof(WAVHeader)];

        tmp->chunkID=toBig(0x52494646);
        tmp->format=toBig(0x57415645);
        tmp->subChunk1ID=toBig(0x666d7420);
        tmp->subChunk1Size=16;//pcm
        tmp->audioFormat=1;
        tmp->subChunk2ID=toBig(0x64617461);

        tmp->bitsPerSample=BitsPerSample;//low bits per sample
        tmp->numChannels=NumChannels;//mono
        tmp->sampleRate=SampleRate;//low sample rate
        tmp->byteRate=tmp->sampleRate*tmp->numChannels*tmp->bitsPerSample/8;
        tmp->blockAlign=tmp->numChannels*tmp->bitsPerSample/8;//number of bytes for one sample
        tmp->subChunk2Size=tmp->numChannels*NumSamples*tmp->bitsPerSample/8;//Number Of Bytes In Data
        tmp->chunkSize= 4 + (8 + tmp->subChunk1Size) + (8 + tmp->subChunk2Size);
    }
    Wav::~Wav(){
        if(data!=NULL)
            delete[](data);
        data=NULL;
        wavpoint=NULL;
        wavsize=0;
    }
    Wav::Wav(const Wav& in){
        *this=in;
    }
    Wav& Wav::operator=(const Wav& in){
        if(this!=&in){
            if (data!=NULL)
                delete[](data);
            wavpoint=in.wavpoint;
            wavsize=in.wavsize;
            memcpy(data,in.data,in.wavsize);
        }
        return *this;
    }
    void playSine(double hz, int duration, double volume){
        Wav tmp;
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) duration;
        const double PERIOD = 1 / hz;
        const double TIME_STEP = 1 / (double)(samples-100);
        double time=0;

        tmp.setupHeader(samples,16,1,(DWORD)srate);

        for(int a = 0; a < 50; a++){
            double angle = (2 * M_PI / PERIOD) * time;
            double factor = 0.5 * (sin(angle) + 1);
            int x = int(volume*((double)(a)/50) * factor);
            ((unsigned short*)tmp.wavpoint)[a] = x;
            time += TIME_STEP;
        }
        for(int a = 50; a < samples-50; a++){
            double angle = (2 * M_PI / PERIOD) * time;
            double factor = 0.5 * (sin(angle) + 1);
            int x = int(volume * factor);
            ((unsigned short*)tmp.wavpoint)[a] = x;
            time += TIME_STEP;
        }
        for(int a = samples-50,ebb=50; a < samples; a++,ebb--){
            double angle = (2 * M_PI / PERIOD) * time;
            double factor = 0.5 * (sin(angle) + 1);
            int x = int(volume*((double)(ebb)/50) * factor);
            ((unsigned short*)tmp.wavpoint)[a] = x;
            time += TIME_STEP;
        }



        PlaySound((LPCSTR)tmp.data, NULL, SND_MEMORY);//Synch



        //1000ms / 1sec
        //samplerate samples/SEC
        //ms/1000*samplerate = sample count?


    }
    void playSquare(int hz, int duration){
    }
    void playStatic(int hz, int duration){
    }
    void playSaw(int hz, int duration){
    }
    void stopSound(){
    }
    void playWav(WAVHeader& wav){
    }

    double hzforfreq(int halfstepsfromA3){
        return 220 * pow(twelvethroot,halfstepsfromA3);
    }

    std::string getletterforoffstep(int offstep){
        std::string ret;
        int scalestep = floor((double)offstep/12.0) + 3;

        offstep%=12;
        if(offstep<0){
            offstep += 12;
        }
        static int sharpos[] = {1,4,6,9,11};
        bool issharp=0;
        int offset=0;
        for(int a = 0; a < sizeof(sharpos)/sizeof(int); a++){
            if (offstep==sharpos[a]){
                issharp=1;
            }
            if (offstep>sharpos[a]){
                offset++;
            }
        }
        char letter = 'a'+(offstep-offset);
        ret += letter - issharp;
        ret += "(";
        std::stringstream ss;
        ss << scalestep+1;
        ret += ss.str();
        //ret += (scalestep);
        ret += ")";
        if (issharp){
            ret += '#';
            //letter='#';

        //letter+='A'-'a'-1;
        }

        return ret;
    }

    note::note(double ihz,int iduration, notestyle istyle,bool imix){
        scale=key=0;
        hz=ihz;
        duration=iduration;
        style=istyle;
        mix=imix;
        branch=0;position=0;
    }

    note::note(double ihz, int iposition, int ibranch, int iduration, notestyle istyle){
        scale=key=0;
        hz=ihz;
        duration=iduration;
        position=iposition;
        branch=ibranch;
        style=istyle;
        mix=false;
    }
    note::note(int iscale, int ikey, int iposition, int ibranch, int iduration, notestyle istyle){
        scale=iscale;
        key=ikey;

        hz=hzforfreq(iscale*12+ikey);
        duration=iduration;
        position=iposition;
        branch=ibranch;
        style=istyle;
        mix=false;
    }


    int addSineToWav(note& note, int volume, Wav& wav, int curpos){
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) note.duration;
        const double PERIOD = 1 / note.hz;
        const double TIME_STEP = 1 / (double)(samples-100);
        double time=0;
        if (note.mix){
            curpos-=samples;
        }

        for(int a = 0; a < samples; a++){
            double angle = (2 * M_PI / PERIOD) * time;
            double factor = 0.5 * (sin(angle) + 1);
            int x = int(volume * factor);
            if (note.mix){
                ((unsigned short*)wav.wavpoint)[curpos] += x;
            }
            else{
                ((unsigned short*)wav.wavpoint)[curpos] = x;
            }
            curpos++;
            time += TIME_STEP;
        }
        return curpos;
    }
    int addSquareToWav(note& note,int volume, Wav& wav, int curpos){
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) note.duration;
        const double PERIOD = 1 / note.hz;
        const double TIME_STEP = 1 / (double)(samples);
        double time=0;
        if (note.mix){
            curpos-=samples;
        }
        int halfper = (int)((srate*PERIOD)/2);
        double factor = 1;
        for(int a = 0; a < samples; a++){
            if (a%halfper==0){
                factor*=-1;
            }
            int x = int(volume * factor);

            if (note.mix){
                ((unsigned short*)wav.wavpoint)[curpos] += x;
            }
            else{
                ((unsigned short*)wav.wavpoint)[curpos] = x;
            }
            curpos++;
            time += TIME_STEP;
        }
        return curpos;

    }
    int addSawToWav(note& note,int volume, Wav& wav, int curpos){
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) note.duration;
        const double PERIOD = 1 / note.hz;
        const double TIME_STEP = 1 / (double)(samples-100);
        double time=0;
        double phasor=0;
        if (note.mix){
            curpos-=samples;
        }
        for(int a = 0; a < samples; a++){
            phasor += 2.0 * (1.0/(srate/ note.hz));
            if (phasor > 1.0) {
                phasor-=2.0;
            }
            int x = int(volume * phasor);


            if (note.mix){
                ((unsigned short*)wav.wavpoint)[curpos] += x;
            }
            else{
                ((unsigned short*)wav.wavpoint)[curpos] = x;
            }
            curpos++;
            time += TIME_STEP;
        }
        return curpos;
    }
    int addSilenceToWav(note& note,int volume, Wav& wav, int curpos){
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) note.duration;
        if (note.mix){
            curpos-=samples;
        }
        for(int a = 0; a < samples; a++){
            ((unsigned short*)wav.wavpoint)[curpos] = 0;
            curpos++;
        }
        return curpos;
    }
    int addStaticToWav(note& note,int volume, Wav& wav, int curpos){
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) note.duration;
        const double PERIOD = 1 / note.hz;
        const double TIME_STEP = 1 / (double)(samples-100);
        double time=0;
        if (note.mix){
            curpos-=samples;
        }
        for(int a = 0; a < samples; a++){
            double phasor = (double)(rand()%10000)/5000.0 - 1.0;
            int x = int(volume * phasor);

            if (note.mix){
                ((unsigned short*)wav.wavpoint)[curpos] += x;
            }
            else{
                ((unsigned short*)wav.wavpoint)[curpos] = x;
            }
            curpos++;
            time += TIME_STEP;
        }
        return curpos;
    }
    int addNoteToWav(note& note,int volume, Wav& wav, int curpos){
        if (note.style==notestyle::noteSine){
            return addSineToWav(note,volume,wav,curpos);
        }
        if (note.style==notestyle::noteSquare){
            return addSquareToWav(note,volume,wav,curpos);
        }
        if (note.style==notestyle::noteSaw){
            return addSawToWav(note,volume,wav,curpos);
        }
        if (note.style==notestyle::noteSilence){
            return addSilenceToWav(note,volume,wav,curpos);
        }
        if (note.style==notestyle::noteStatic){
            return addStaticToWav(note,volume,wav,curpos);
        }
    }
    int getSongDuration(std::vector<note>& song){
        int totalms = 0;
        for(int a = 0; a < song.size(); a++){
            if (song[a].mix){
                if (song[a].duration > totalms){
                    totalms=song[a].duration;
                }
            }
            else{
                totalms+=song[a].duration;
            }
        }
        return totalms;
    }
    Wav makewav(std::vector<note> song, int volume){
        int totalms=getSongDuration(song);
        Wav tmp;
        double srate = 44100;
        DWORD samples = srate / 1000.0 * (double) totalms;
        tmp.setupHeader(samples,16,1,(DWORD)srate);

        int curpos=0;
        for(int a = 0; a < song.size(); a++){
            curpos = addNoteToWav(song[a],volume,tmp,curpos);
        }

        return tmp;
    }

    void playSong(std::vector<note> song, int volume, bool repeated){
        Wav tmp = makewav(song,volume);
        unsigned long flags = SND_MEMORY;
        PlaySound((LPCSTR)tmp.data, NULL, flags);
    }
    struct FullSong{
        std::vector<note> song;
        int volume;
        int ready;
        bool repeat;
    };
    void asyncPlayer(void* fullSong = NULL){
        if (fullSong!=NULL){
            FullSong fs = *(FullSong*)fullSong;
            ((FullSong*)fullSong)->ready=true;
            playSong(fs.song,fs.volume,fs.repeat);
        }
        return;
    }
    void playSongAsync(std::vector<note> song, int volume, bool repeated){
        FullSong param;
        param.ready=false;
        param.song=song;
        param.volume=volume;
        param.repeat=repeated;

        ulti::newthread(asyncPlayer,(void*)&param);
        while(!param.ready)
            Sleep(1);
    }
    void saveSong(std::vector<note> song, int volume, const std::string& filename){
        Wav tmp = makewav(song,volume);
        ulti::savefile("test.wav",tmp.data,tmp.wavsize);
    }




};
