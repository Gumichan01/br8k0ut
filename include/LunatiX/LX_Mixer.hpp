
/*
*   Copyright © 2017 Luxon Jean-Pierre
*   https://gumichan01.github.io/
*
*   LunatiX is a free, SDL2-based library.
*   It can be used for open-source or commercial games thanks to the zlib/libpng license.
*
*   Luxon Jean-Pierre (Gumichan01)
*   luxon.jean.pierre@gmail.com
*/

#ifndef LX_MIXER_H_INCLUDED
#define LX_MIXER_H_INCLUDED

/**
*   @file LX_Mixer.hpp
*   @brief The mixer library
*   @author Luxon Jean-Pierre(Gumichan01)
*   @version 0.10
*
*/

#include <LunatiX/utils/utf8_string.hpp>

const int LX_MIXER_AUDIO_FREQUENCY = 44100;     /**< The default audio frequency             */
const int LX_MIXER_STEREO_SOUND = 2;            /**< The stereo variable for the mix namespace  */
const int LX_MIXER_MONO_SOUND = 1;              /**< The mono variable for the mix namespace    */
const int LX_MIXER_DEFAULT_CHUNKSIZE = 1024;    /**< The default chunsize for the mix namespace */

const uint8_t LX_MIXER_EFFECT_LOUD = 255;        /**< Loud (for effect functions)    */
const uint8_t LX_MIXER_EFFECT_SILENCE = 0;       /**< Silence (for effect functions) */
const uint8_t LX_MIXER_EFFECT_NO_DISTANCE = 0;   /**< The distance between the source and the listener */
const uint16_t LX_MIXER_EFFECT_NO_ANGLE = 0;     /**< The angle between the source and the front */


namespace LX_FileIO
{
class LX_FileBuffer;
}


/**
*   @ingroup Audio
*   @namespace LX_Mixer
*   @brief The audio namespace
*
*   It handles sound mixing and effect processing
*
*   @note The effect defined in these following functions:
*
*               - setPanning()
*               - setPosition()
*               - setDistance()
*               - reverseStereo()
*
*   is set as a post-mix processing, i.e. the post-mix processor runs
*   after every specific post-mixers set by the functions listed before.
*
*   If a mix processor has been defined for a specific channel
*   for one of these functions, so this processor is run before every post-mixers.
*
*   Example:
*
*               int chan = 5;                           // channel number
*               LX_Mixer::LX_Chunk chunk("test.wav");
*               LX_Mixer::setDistance(100);             // distance as post-processing effect
*               LX_Mixer::setPanning(55,200);           // panning as post-processing effect
*               LX_Mixer::setPanning(chan,255,0);       // panning on a specific channel
*               chunk.play(chan);
*               //  So the order of post-mix processing is:
*               //    ① panning on channel #5
*               //    ② panning (post-processing)
*               //    ③ distance (post-processing)
*
*
*   @warning In order to use this namespace, the *audio* flag
*   in the configuration file must be set to 1, otherwise the behaviour of
*   the library is undefined.
*
*/
namespace LX_Mixer
{

class LX_Music;
class LX_Chunk;

/* == Volume == */

/**
*   @fn void setOverallVolume(unsigned short volume)
*
*   Set the overall volume
*
*   @param [in] volume the volume between 0 and 100
*
*   @note if volume > 100, then the overall volume is set to 100
*/
void setOverallVolume(unsigned short volume);
/**
*   @fn void setMusicVolume(unsigned short pvolume)
*
*   Set the music volume in percentage of the overall volume
*
*   @param [in] pvolume the volume of the music in percentage of the overall volume
*
*   @note If the overall volume is 60 and pvolume is 50%, the music volume
*        is 50 * 60 / 100.
*   @note The music volume ca be get using ::getMusicVolume()
*/
void setMusicVolume(unsigned short pvolume);
/**
*   @fn void setFXVolume(unsigned short pvolume)
*
*   Set the FX volume in percentage of the overall volume
*
*   @param [in] pvolume the volume of the music in percentage of the overall volume
*
*   @note Example:
*        If the overall volume is 60 and pvolume is 50%, the FX volume
*        is 50 * 60 / 100.
*   @note The FX volume ca be get using ::getFXVolume()
*/
void setFXVolume(unsigned short pvolume);

/**
*   @fn unsigned short getOverallVolume()
*   Get the overall volume
*   @return The overall volume
*/
unsigned short getOverallVolume();
/**
*   @fn unsigned short getMusicVolume()
*   Get the music volume
*   @return The music volume
*/
unsigned short getMusicVolume();
/**
*   @fn unsigned short getFXVolume()
*   Get the FX volume
*   @return The FX volume
*/
unsigned short getFXVolume();


/* == Music and chunk == */

/**
*   @fn LX_Chunk * loadSample(LX_FileIO::LX_FileBuffer& file)
*
*   Create a new LX_Chunk instance from a file buffer
*
*   @param [in] file The file buffer
*
*   @return A valid instance of LX_Chunk if the file buffer is valid
*          a null pointer otherwise
*
*   @note   This function creates a new instance of LX_Chunk.
*          So do not forget to destroy it.
*/
LX_Chunk * loadSample(LX_FileIO::LX_FileBuffer& file);

/**
*   @fn void setMusicPosition(double pos)
*   Set the position of the currently playing song from the beginning
*
*   @param [in] pos The position to play from, in second
*
*   @note This function does not works on every music sources.
*   @note Here are the music sources available:
*        - MOD
*        - OGG
*        - MP3
*/
void setMusicPosition(double pos);

/**
*   @fn int allocateChannels(int num)
*
*   Set the number of channel to use.
*
*   @param [in] num The number of channels to allocate
*   @return The number of channels allocated
*
*   @note   If *num* is 0, then all channels will be freed.
*   @note   A negative number do nothing,
*          it just returns the number of allocated channels.
*   @note   If *num* is less than the current number of channels,
*          then the higher channels will be stopped, freed, and not mixed.
*/
int allocateChannels(int num);
/**
*   @fn int reserveChannels(int numchans)
*
*   Set the number of channel to reserve.
*
*   @param [in] numchans The number of channels to reserve
*
*   @return The number of channels reserved
*   @note If *num* is 0, then all channels will be unreserved
*/
int reserveChannels(int numchans);

/**
*   @fn bool groupChannel(int chan, int tag)
*
*   Add a channel to a specific group
*
*   @param [in] chan The channel to assign the tag
*   @param [in] tag The tag that define the group to put the channel. Positive value
*
*   @return TRUE on success, FALSE otherwise
*   @note Setting -1 in *tag* put the channel in the default group
*/
bool groupChannel(int chan, int tag);
/**
*   @fn int groupChannels(int from, int to, int tag)
*
*   Add a channel to a specific group
*
*   @param [in] from First Channel number of channels to assign tag to. Must be less or equal to to
*   @param [in] to Last Channel number of channels to assign tag to. Must be greater or equal to from
*   @param [in] tag The tag that define the group to put the channel. Positive value
*
*   @return The number of tagged channels on success.
*   @note If that number is less than to-from+1 then
*          some channels were no tagged because they didn't exist
*   @note Setting -1 in *tag* reset the group to the default channel
*/
int groupChannels(int from, int to, int tag);

/**
*   @fn int groupCount(int tag)
*
*   Get the number of channels in the group specified by the tag
*
*   @param [in] tag The group id
*
*   @return The number of channels found in the group. This function never fails
*   @note Setting -1 in *tag* will count all channels
*/
int groupCount(int tag);
/**
*   @fn int channelAvailable(int tag)
*
*   Get the first available (not playing) channel in the group specified by the tag
*
*   @param [in] tag The group id to look for the available channel
*
*   @return The channel id on success. -1 if no channel is available.
*/
int channelAvailable(int tag);

/**
*   @fn bool groupPlayChunk(LX_Chunk& chunk, int tag, int loops = 0)
*
*   Play the chunk on a channel of the group specified by the tag
*
*   @param [in] chunk The chunk to play
*   @param [in] tag The group id to look for the channel for playing the chunk on
*   @param [in] loops Optional argument tha specified the number of loop (default value = 0)
*
*   @return TRUE if the chunk can be played, FALSE if no channel is available
*
*   @note If the group is empty, any unreserved channels in the default
*        group is selected and the chunk is played on it
*   @note If no channel of the group is available for playing, the oldest
*        playing channel is chosen. So, it is halted, and is used to play the chunk on
*/
bool groupPlayChunk(LX_Chunk& chunk, int tag, int loops = 0);

/**
*   @fn void pause(int channel)
*
*   Pause the channel
*
*   @param [in] channel The channel to pause
*   @note If channel is -1, then all channels will be paused
*/
void pause(int channel);
/**
*   @fn void resume(int channel)
*
*   Unpause the channel
*
*   @param [in] channel The channel to resume playing
*   @note If channel is -1, then all channels will be unpaused
*/
void resume(int channel);

/**
*   @fn void haltChannel(int channel)
*
*   Halt the channel playback
*
*   @param [in] channel The channel to stop playing
*   @note If channel is -1, then all channels will be stopped
*/
void haltChannel(int channel);
/**
*   @fn void expireChannel(int channel, int ticks)
*
*   Halt the channel playback after some milliseconds
*
*   @param [in] channel The channel to stop playing
*   @param [in] ticks The time in millisecond
*   @note If channel is -1, then all channels will be stopped
*/
void expireChannel(int channel, int ticks);

/**
*   @fn int isPlaying(int channel)
*
*   Check if the channel is playing
*
*   @param [in] channel The channel to test
*   @return 0 If the channel is not playing, 1 otherwise
*
*   @note If channel is -1, then all channels will be tested
*          and the number of channels playing is returned
*/
int isPlaying(int channel);
/**
*   @fn int isPaused(int channel)
*
*   Check if the channel is paused
*
*   @param [in] channel The channel to test
*   @return 0 If the channel is not paused, 1 otherwise
*
*   @note If channel is -1, then all channels will be tested
*          and the number of aused channels is returned
*/
int isPaused(int channel);


/* == Effects == */

/**
*   @fn void fadeInMusic(LX_Music& music, int ms)
*
*   Fade in the loaded Music over some milliseconds of time
*
*   @param [in] music The music to fade in
*   @param [in] ms Milliseconds for the fade-in effect to complete
*
*   @note fadeInMusic starts playing the music with the fade-in effect.
*        It is not necessary to call LX_Music::play() if this function is called
*
*   @note Any previous music will be halted, or if it is fading out
*          it will wait (blocking) for the fade to complete
*/
void fadeInMusic(LX_Music& music, int ms);
/**
*   @fn void void fadeInMusicPos(LX_Music& music,int ms, int pos)
*
*   Fade in the loaded Music over some milliseconds of time from the position
*
*   @param [in] music The music to fade in
*   @param [in] ms Milliseconds for the fade-in effect to complete
*   @param [in] pos The position to start the music
*
*   @note fadeInMusicPos starts playing the music with the fade-in effect.
*        It is not necessary to call LX_Music::play() if this function is called
*
*   @note Any previous music will be halted, or if it is fading out
*          it will wait (blocking) for the fade to complete
*/
void fadeInMusicPos(LX_Music& music, int ms, int pos);
/**
*   @fn void fadeOutMusic(int ms)
*
*   Fade out the loaded Music over some milliseconds of time
*
*   @param [in] ms Milliseconds for the fade-out effect to complete
*
*   @note This functions works only when music is playing and
*          no fading is already set to fade out
*/
void fadeOutMusic(int ms);

/**
*   @fn void setPanning(uint8_t left, uint8_t right)
*
*   Set the panning, increasing of decreasing the volume on the left or the right,
*   as a post-processing effect
*
*   @param [in] left The volume of the left audio channel (0 - 255)
*   @param [in] right The volume of the right audio channel (0 - 255)
*
*   @note 1 — This function set the effect on every mixing channels.
*        The other signature can be used to set the effect on a specific channel.
*   @note 2 — The easiest way to do true panning is to call setPanning(left, 254 - left),
*        so that the total volume is correct, if you consider
*        the maximum volume to be 127 per channel for center,
*        or 254 max for left, this works, but about halves the effective volume.
*   @note 3 — To unregister this effect, use this function with 255 as left and right value
*        or simply use LX_Mixer::removePanning().
*/
void setPanning(uint8_t left, uint8_t right);
/**
*   @fn void setPanning(int chan, uint8_t left, uint8_t right)
*
*   Set the panning, increasing of decreasing the volume on the left or the right,
*   on a specific mixing channel
*
*   @param [in] chan The channel
*   @param [in] left The volume of the left audio channel (0 - 255)
*   @param [in] right The volume of the right audio channel (0 - 255)
*
*   @note 1 — The easiest way to do true panning is to call setPanning(left, 254 - left),
*        so that the total volume is correct, if you consider
*        the maximum volume to be 127 per channel for center,
*        or 254 max for left, this works, but about halves the effective volume.
*   @note 2 — To unregister this effect, use this function with 255 as left and right value
*        or simply use LX_Mixer::removePanning(int chan).
*/
void setPanning(int chan, uint8_t left, uint8_t right);
/**
*   @fn void removePanning()
*   Remove the panning effect applied on every channels
*/
void removePanning();
/**
*   @fn void removePanning(int chan)
*   Remove the panning effect on a specific channel
*   @param [in] chan The channel to remove the effect from
*   @note This function also remove the post-processing effects
*/
void removePanning(int chan);

// 3D Position

/**
*   @fn void setPosition(int16_t angle)
*
*   Set the virtual position of the audio source.
*
*   @param [in] angle The angle between 0 and 360, larger angles are reduced using angle % 360
*   @note This function call setPosition(angle, LX_MIXER_EFFECT_NO_DISTANCE)
*/
void setPosition(int16_t angle);
/**
*   @fn void setPosition(int16_t angle, uint8_t distance)
*
*   Set the virtual position of the audio source (post-processing).
*
*   @param [in] angle The angle between 0 and 360, larger angles are reduced using angle % 360
*   @param [in] distance The distance between the source and the listener
*/
void setPosition(int16_t angle, uint8_t distance);
/**
*   @fn void setPosition(int chan, int16_t angle, uint8_t distance)
*
*   Set the virtual position of the audio source in a specific channel.
*
*   @param [in] chan The channel
*   @param [in] angle The angle between 0 and 360, larger angles are reduced using angle % 360
*   @param [in] distance The distance between the source and the listener
*/
void setPosition(int chan, int16_t angle, uint8_t distance);
/**
*   @fn void resetPosition()
*   Reset the virtual position of the audio source.
*/
void resetPosition();
/**
*   @fn void resetPosition(int chan)
*   Reset the virtual position of the audio source.
*   @param [in] chan The channel
*/
void resetPosition(int chan);

// Reverse stereo

/**
*   @fn void reverseStereo(bool flip)
*
*   Reverse the left and right audio channels.
*
*   @param [in] flip TRUE to set the effect, FALSE to unregister the effect
*   @note If you want to unregister the effect on a channel, set false as flip
*/
void reverseStereo(bool flip);
/**
*   @fn void reverseStereo(int chan, bool flip)
*
*   Reverse the left and right audio channels on a specific mix channel
*
*   @param [in] chan The channel to reverse the stereo sound from
*   @param [in] flip TRUE to set the effect, FALSE to unregister the effect
*   @note If you want to unregister the effect on a channel, set false as flip
*/
void reverseStereo(int chan, bool flip);

// Distance

/**
*   @fn void setDistance(uint8_t distance)
*   Set the distance to all channels
*   @param [in] distance The virtual distance between the user and the source
*/
void setDistance(uint8_t distance);
/**
*   @fn void setDistance(int chan, uint8_t distance)
*   Set the distance to a specific channel
*   @param [in] chan The channel
*   @param [in] distance The virtual distance between the user and the source
*/
void setDistance(int chan, uint8_t distance);

}

#endif // LX_MIXER_H_INCLUDED
