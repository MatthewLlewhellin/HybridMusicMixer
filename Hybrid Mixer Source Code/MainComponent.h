#pragma once

#include <JuceHeader.h>
#include "ThumbnailComp.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::ChangeListener, 
                      public juce::MixerAudioSource,
                      public juce::ChangeBroadcaster,
                      private juce::Timer,
                      private juce::FileBrowserListener
    //filebrowserlistenever added for thumbnail comp
{
public:
    //==============================================================================
    MainComponent();

    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void releaseResources() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    //==============================================================================
    //thumbnail components below:

private:
    //==============================================================================
    // Your private member variables go here...
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Pausing,
        Paused
    };

    enum TransportState2 
    {
        Stopped2,
        Starting2,
        Playing2,
        Stopping2,
        Pausing2,
        Paused2
    };

    juce::TextButton playButton;
    juce::TextButton playButton2;

    juce::TextButton stopButton;
    juce::TextButton stopButton2;

    juce::ToggleButton loopingToggle;
    juce::ToggleButton loopingToggle2;

    juce::Label currentPositionLabel;
    juce::Label currentPositionLabel2;

    void timerCallback();

    void changeState(TransportState newState);
    void changeState2(TransportState2 newState2);

    void updateLoopState(bool shouldLoop);
    void updateLoopState2(bool shouldLoop2);

    void playButtonClicked();
    void playButtonClicked2();

    void stopButtonClicked();
    void stopButtonClicked2();

    void loopButtonChanged();
    void loopButtonChanged2();

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource, readerSource2;
    juce::AudioTransportSource transportSource, transportSource2;

    TransportState state;
    TransportState2 state2;
    MixerAudioSource mixer;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail, thumbnail2;
    //==============================================================================
    //thumbnail components below

    void showAudioResource(juce::URL resource);
    void showAudioResource2(juce::URL resource2);
    bool loadURLIntoTransport(const juce::URL& audioURL);
    bool loadURLIntoTransport2(const juce::URL& audioURL2);
    void updateFollowTransportState();
    void updateFollowTransportState2();
    void selectionChanged() override;

    void fileClicked(const juce::File&, const juce::MouseEvent&) override;
    void fileDoubleClicked(const juce::File&) override;
    void browserRootChanged(const juce::File&) override;

    juce::TimeSliceThread thread{ "audio file preview" }, thread2{ "audio file preview" };
    juce::DirectoryContentsList directoryList{ nullptr, thread }, directoryList2{ nullptr, thread2 };
    juce::FileTreeComponent fileTreeComp{ directoryList }, fileTreeComp2{ directoryList2 };
    //juce::Label explanation{ {}, "Sample" };

    juce::URL currentAudioFile, currentAudioFile2;
    juce::AudioSourcePlayer audioSourcePlayer;
    //added a second pointer to thumnailcomp below
    std::unique_ptr<ThumbnailComp> thumbnailComp, thumbnailComp2;
    juce::Label zoomLabel{ {}, "Zoom:" }, zoomLabel2{ {}, "Zoom:" }, crossFaderLabel{ {}, "Crossfade:" };
    juce::Slider zoomSlider{ juce::Slider::LinearHorizontal, juce::Slider::NoTextBox }, 
        zoomSlider2{ juce::Slider::LinearHorizontal, juce::Slider::NoTextBox }, 
        crossFader{ juce::Slider::LinearHorizontal, juce::Slider::NoTextBox };
    juce::ToggleButton followTransportButton{ "Follow Transport" }, followTransportButton2{ "Follow Transport" };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
