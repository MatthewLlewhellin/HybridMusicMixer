#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :state(Stopped), 
                                state2(Stopped2), 
                                thumbnailCache(5), 
                                thumbnail(512, formatManager, thumbnailCache),
                                thumbnail2(512, formatManager, thumbnailCache) 
{

    thumbnail.addChangeListener(this);
    thumbnail2.addChangeListener(this);

    //==========================================================================
    /*
    //Initialising the play button
    //settings its text as "Play"
    //when the play button is clicked it calls the playButtonClicked() function
    //setting the colour of the button to be green
    //play button is not enabled
    */
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    /*
    //Initialising the stop button
    //settings its text as "Stop"
    //when the stop button is clicked it calls the stopButtonClicked() function
    //setting the colour of the button to be red
    //stop button is not enabled
    */
    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    /*
    //Initialising the looping toggle
    //settings its text as "Loop"
    //when the looping toggle is clicked it calls the loopButtonChanged() function
    */
    addAndMakeVisible(&loopingToggle);
    loopingToggle.setButtonText("Loop");
    loopingToggle.onClick = [this] {loopButtonChanged();};

    /*
    //Initialising the current position label
    //settings its text as "Stopped"
    */
    addAndMakeVisible(&currentPositionLabel);
    currentPositionLabel.setText("Stopped", juce::dontSendNotification);
    //==========================================================================
    /*
    //Initialising the play button 2
    //settings its text as "Play"
    //when the play button is clicked it calls the playButtonClicked2() function
    //setting the colour of the button to be green
    //play button is not enabled
    */
    addAndMakeVisible(&playButton2);
    playButton2.setButtonText("Play");
    playButton2.onClick = [this] { playButtonClicked2(); };
    playButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton2.setEnabled(false);

    /*
    //Initialising the stop button2
    //settings its text as "Stop"
    //when the stop button is clicked it calls the stopButtonClicked2() function
    //setting the colour of the button to be red
    //stop button is not enabled
    */
    addAndMakeVisible(&stopButton2);
    stopButton2.setButtonText("Stop");
    stopButton2.onClick = [this] { stopButtonClicked2(); };
    stopButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton2.setEnabled(false);

    /*Displaying the second looping toggle on the application
    * setting the text of the toggle to "Loop" to show the user
    * if the toggle is clicked it calls to the function "loopButtonChanged2()"*/
    addAndMakeVisible(&loopingToggle2);
    loopingToggle2.setButtonText("Loop");
    loopingToggle2.onClick = [this] {loopButtonChanged2();};

    /*Displaying the second current position label on the application
    * setting the text of the label to "Stopped" to show the audio file
    * is not currently playing.*/
    addAndMakeVisible(&currentPositionLabel2);
    currentPositionLabel2.setText("Stopped", juce::dontSendNotification);
    //========================================================================== 
    //all code beneath is related to the thumbnail
    addAndMakeVisible(zoomLabel);
    zoomLabel.setFont(juce::Font(15.00f, juce::Font::plain));
    zoomLabel.setJustificationType(juce::Justification::centredRight);
    zoomLabel.setEditable(false, false, false);
    zoomLabel.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    zoomLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    addAndMakeVisible(zoomLabel2);
    zoomLabel2.setFont(juce::Font(15.00f, juce::Font::plain));
    zoomLabel2.setJustificationType(juce::Justification::centredRight);
    zoomLabel2.setEditable(false, false, false);
    zoomLabel2.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    zoomLabel2.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    addAndMakeVisible(crossFaderLabel);
    crossFaderLabel.setFont(juce::Font(15.00f, juce::Font::plain));
    crossFaderLabel.setJustificationType(juce::Justification::centredRight);
    crossFaderLabel.setEditable(false, false, false);
    crossFaderLabel.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    crossFaderLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    addAndMakeVisible(followTransportButton);
    followTransportButton.onClick = [this] { updateFollowTransportState(); };

    addAndMakeVisible(followTransportButton2);
    followTransportButton2.onClick = [this] { updateFollowTransportState2(); };

    addAndMakeVisible(fileTreeComp);
    addAndMakeVisible(fileTreeComp2);

    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userHomeDirectory), true, true);
    directoryList2.setDirectory(juce::File::getSpecialLocation(juce::File::userHomeDirectory), true, true);

    fileTreeComp.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::lightgrey.withAlpha(0.6f));
    fileTreeComp.addListener(this);

    fileTreeComp2.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::lightgrey.withAlpha(0.6f));
    fileTreeComp2.addListener(this);

    addAndMakeVisible(zoomSlider);
    zoomSlider.setRange(0, 1, 0);
    zoomSlider.onValueChange = [this] { thumbnailComp->setZoomFactor(zoomSlider.getValue()); };
    zoomSlider.setSkewFactor(2);


    addAndMakeVisible(zoomSlider2);
    zoomSlider2.setRange(0, 1, 0);
    zoomSlider2.onValueChange = [this] { thumbnailComp2->setZoomFactor(zoomSlider2.getValue()); };
    zoomSlider2.setSkewFactor(2);

    //initialising the crossfader and setting up the crossfader functions
    //setting the gain for transportSource to increase when transportSource2 decreases, with increments of 0.1
    addAndMakeVisible(crossFader);
    crossFader.setRange(0, 1, 0.1);
    crossFader.onValueChange = [this] {transportSource.setGain(1 - crossFader.getValue()), transportSource2.setGain(crossFader.getValue()); };
    crossFader.setValue(0.5); //setting the slider to be in the middle

    //Pointer toward the ThumbnailComp.cpp
    //creating an instance of the ThumbnailComp class
    thumbnailComp.reset(new ThumbnailComp(formatManager, transportSource, zoomSlider));
    addAndMakeVisible(thumbnailComp.get());
    thumbnailComp->addChangeListener(this);

    //Pointer toward the ThumbnailComp.cpp
    //creating an instance of the ThumbnailComp class
    thumbnailComp2.reset(new ThumbnailComp(formatManager, transportSource2, zoomSlider2));
    addAndMakeVisible(thumbnailComp2.get());
    thumbnailComp2->addChangeListener(this);

    //used for the file manager
    //starts a thread with priority of 3/10
    thread.startThread(3);
    thread2.startThread(3);
    //========================================================================== 
    
    //Used to define the size of the application.
    setSize(1280, 720);
    //setting the application bbackground to opaque
    setOpaque(true);
    //registers the formats that come with JUCE
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    transportSource2.addChangeListener(this);

    // Specify the number of input and output channels that we want to open
    setAudioChannels(2, 2);
    startTimer(20);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
//function that is called to initialise any audio playing objects
//initialising the mixer
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
        mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixer.getNextAudioBlock(bufferToFill);
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
}
//==============================================================================


void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transportSource) {
        if (transportSource.isPlaying())
            changeState(Playing);
        else if ((state==Stopping)||(state==Playing))
            changeState(Stopped);
        else if (Pausing == state)
            changeState(Paused);
    }
    if (source == &transportSource2) {
        if (transportSource2.isPlaying())
            changeState2(Playing2);
        else if ((state2 == Stopping2) || (state2 == Playing2))
            changeState2(Stopped2);
        else if (Pausing2 == state2)
            changeState2(Paused2);
    }
    if (source == thumbnailComp.get())
        showAudioResource(juce::URL(thumbnailComp->getLastDroppedFile()));
    if (source == thumbnailComp2.get())
        showAudioResource2(juce::URL(thumbnailComp2->getLastDroppedFile()));
}

void MainComponent::timerCallback() {
    if (transportSource.isPlaying()) {
        juce::RelativeTime position(transportSource.getCurrentPosition());

        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;
        auto millis = ((int)position.inMilliseconds()) % 1000;
        auto positionString = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);//converting to formatted string

        currentPositionLabel.setText(positionString, juce::dontSendNotification);

    }
    if (transportSource2.isPlaying()) {
        juce::RelativeTime position(transportSource2.getCurrentPosition());

        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;
        auto millis = ((int)position.inMilliseconds()) % 1000;
        auto positionString = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);

        currentPositionLabel2.setText(positionString, juce::dontSendNotification);

    }
}
//==============================================================================
//switch statemnt that changes the transport state
void MainComponent::changeState(TransportState newState) {
    if (state != newState) {
        state = newState;

        switch (state) {
        case Stopped:
            playButton.setButtonText("Play");
            stopButton.setButtonText("Stop");
            stopButton.setEnabled(false);
            transportSource.setPosition(0.0);
            break;

        case Stopping:
            transportSource.stop();
            break;

        case Starting:
            transportSource.start();
            break;

        case Playing:
            playButton.setButtonText("Pause");
            stopButton.setButtonText("Stop");
            stopButton.setEnabled(true);
            break;

        case Pausing:
            transportSource.stop();
            break;

        case Paused:
            playButton.setButtonText("Resume");
            stopButton.setButtonText("Restart");
            break;
        }
    }
}
//switch statemnt that changes the transport state
void MainComponent::changeState2(TransportState2 newState) {
    if (state2 != newState) {
        state2 = newState;

        switch (state2) {
        case Stopped2:
            playButton2.setButtonText("Play");
            stopButton2.setButtonText("Stop");
            stopButton2.setEnabled(false);
            transportSource2.setPosition(0.0);
            break;

        case Stopping2:
            transportSource2.stop();
            break;

        case Starting2:
            transportSource2.start();
            break;

        case Playing2:
            playButton2.setButtonText("Pause");
            stopButton2.setButtonText("Stop");
            stopButton2.setEnabled(true);
            break;

        case Pausing2:
            transportSource2.stop();
            break;

        case Paused2:
            playButton2.setButtonText("Resume");
            stopButton2.setButtonText("Restart");
            break;
        }
    }
}
//==============================================================================
void MainComponent::updateLoopState(bool shouldLoop) {
    if (readerSource.get() != nullptr)//if there readersource does not equal nullptr
        readerSource->setLooping(shouldLoop);//enable looping
}

void MainComponent::updateLoopState2(bool shouldLoop2) {
    if (readerSource2.get() != nullptr)//if there readersource does not equal nullptr
        readerSource2->setLooping(shouldLoop2);//enable looping
}
//==============================================================================
void MainComponent::playButtonClicked() {
    if ((state == Stopped) || (state == Paused))//if the state is either stopped or paused
        changeState(Starting);//change the state to starting
    else if (state == Playing)//else is the state is playing
        changeState(Pausing);//change the state to pausing
}

void MainComponent::playButtonClicked2() {
    if ((state2 == Stopped2) || (state2 == Paused2))//if the state is either stopped or paused
        changeState2(Starting2);//change the state to starting
    else if (state2 == Playing2)//else is the state is playing
        changeState2(Pausing2);//change the state to pausing
}
//==============================================================================
void MainComponent::stopButtonClicked() {
    if (state == Paused) {//if the state is paused
        changeState(Stopped);//change the state to stopped
        currentPositionLabel.setText("Stopped", juce::dontSendNotification);
    }
    else {
        changeState(Stopping);//change the state to stopping
        currentPositionLabel.setText("Stopped", juce::dontSendNotification);
    }
}
void MainComponent::stopButtonClicked2() {
    if (state2 == Paused2) {//if the state is paused
        changeState2(Stopped2);//change the state to stopped
        currentPositionLabel2.setText("Stopped", juce::dontSendNotification);
    }
    else {
        changeState2(Stopping2);//change the state to stopping
        currentPositionLabel2.setText("Stopped", juce::dontSendNotification);
    }
}
//==============================================================================
//function for toggling the looping of the audio track
void MainComponent::loopButtonChanged() {
    updateLoopState(loopingToggle.getToggleState());
}
//function for toggling the looping of the audio track
void MainComponent::loopButtonChanged2() {
    updateLoopState2(loopingToggle2.getToggleState());
}
//==============================================================================

void MainComponent::releaseResources()
{
    mixer.releaseResources();
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    //=========================================================================
    //Setting the boundaries of all the components.
    //using auto to set values so the components are not hardcoded which enables better resizing
    auto r = getLocalBounds().reduced(4);
    auto rBottomBounds = r.removeFromBottom(50);
    rBottomBounds.removeFromLeft(300);
    rBottomBounds.removeFromRight(400);
    auto rRightBounds = r.removeFromRight(r.getWidth() / 2);
    auto controls = r.removeFromBottom(100);
    auto zoom = controls.removeFromTop(25);
    zoomLabel.setBounds(zoom.removeFromLeft(50));
    zoomSlider.setBounds(zoom);
    auto togglesBounds = controls.removeFromTop(25);
    auto togglesBoundsMiddle = togglesBounds.removeFromRight(2*(togglesBounds.getWidth())/ 3);
    auto togglesBoundsRight = togglesBoundsMiddle.removeFromRight(togglesBoundsMiddle.getWidth() / 2);
    followTransportButton.setBounds(togglesBounds);
    loopingToggle.setBounds(togglesBoundsMiddle);
    currentPositionLabel.setBounds(togglesBoundsRight);

    followTransportButton.setBounds(togglesBounds.removeFromTop(25));
    loopingToggle.setBounds(togglesBoundsMiddle.removeFromTop(25));
    currentPositionLabel.setBounds(togglesBoundsRight.removeFromTop(25));
    r.removeFromTop(6);
    thumbnailComp->setBounds(r.removeFromBottom(140));
    r.removeFromBottom(6);
    fileTreeComp.setBounds(r);


    auto trackControls = controls.removeFromTop(50);
    auto trackControlsRight = trackControls.removeFromRight(trackControls.getWidth() / 2);
    stopButton.setBounds(trackControlsRight);
    playButton.setBounds(trackControls);

    auto controls2 = rRightBounds.removeFromBottom(100);
    auto zoom2 = controls2.removeFromTop(25);
    zoomLabel2.setBounds(zoom2.removeFromLeft(50));
    zoomSlider2.setBounds(zoom2);
    auto togglesBounds2 = controls2.removeFromTop(25);
    auto togglesBoundsMiddle2 = togglesBounds2.removeFromRight(2 * (togglesBounds2.getWidth()) / 3);
    auto togglesBoundsRight2 = togglesBoundsMiddle2.removeFromRight(togglesBoundsMiddle2.getWidth() / 2);
    followTransportButton2.setBounds(togglesBounds2);
    loopingToggle2.setBounds(togglesBoundsMiddle2);
    currentPositionLabel2.setBounds(togglesBoundsRight2);

    followTransportButton2.setBounds(togglesBounds2.removeFromTop(25));
    loopingToggle2.setBounds(togglesBoundsMiddle2.removeFromTop(25));
    currentPositionLabel2.setBounds(togglesBoundsRight2.removeFromTop(25));
    rRightBounds.removeFromTop(6);
    thumbnailComp2->setBounds(rRightBounds.removeFromBottom(140));
    rRightBounds.removeFromBottom(6);
    fileTreeComp2.setBounds(rRightBounds);

    auto trackControls2 = controls2.removeFromTop(50);
    auto trackControlsRight2 = trackControls2.removeFromRight(trackControls2.getWidth() / 2);
    stopButton2.setBounds(trackControlsRight2);
    playButton2.setBounds(trackControls2);
    crossFaderLabel.setBounds(rBottomBounds.removeFromLeft(100));
    crossFader.setBounds(rBottomBounds);
}

void MainComponent::showAudioResource(juce::URL resource)
{
    if (loadURLIntoTransport(resource))
        currentAudioFile = std::move(resource);

    zoomSlider.setValue(0, juce::dontSendNotification);
    thumbnailComp->setURL(currentAudioFile);
}


void MainComponent::showAudioResource2(juce::URL resource)
{
    if (loadURLIntoTransport2(resource))
        currentAudioFile2 = std::move(resource);

    zoomSlider2.setValue(0, juce::dontSendNotification);
    thumbnailComp2->setURL(currentAudioFile2);
}

bool MainComponent::loadURLIntoTransport(const juce::URL& audioURL)
{
    // unload the previous file source and delete it..

    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.release();


    juce::AudioFormatReader* reader = nullptr;

    if (reader == nullptr)
        reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)
    {
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));

        // ..and plug it into our transport source
        transportSource.setSource(readerSource.get(),
            32768,                   // tells it to buffer this many samples ahead
            &thread,                 // this is the background thread to use for reading-ahead
            reader->sampleRate);     // allows for sample rate correction
        mixer.addInputSource(&transportSource, true);//adding the source to the mixer
        playButton.setEnabled(true);
        return true;
    }

    return false;
}

bool MainComponent::loadURLIntoTransport2(const juce::URL& audioURL)
{
    // unload the previous file source and delete it..

    transportSource2.stop();
    transportSource2.setSource(nullptr);
    readerSource2.release();   

    juce::AudioFormatReader* reader = nullptr;

    if (reader == nullptr)
        reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)
    {
        readerSource2.reset(new juce::AudioFormatReaderSource(reader, true));

        // ..and plug it into our transport source
        transportSource2.setSource(readerSource2.get(),
            32768,                   // tells it to buffer this many samples ahead
            &thread2,                // this is the background thread to use for reading-ahead
            reader->sampleRate);     // allows for sample rate correction
        mixer.addInputSource(&transportSource2, true);//adding the source to the mixer
        playButton2.setEnabled(true);
        return true;
    }

    return false;
}

void MainComponent::updateFollowTransportState()
{
    thumbnailComp->ThumbnailComp::setFollowsTransport(followTransportButton.getToggleState());

}

void MainComponent::updateFollowTransportState2()
{
    thumbnailComp2->ThumbnailComp::setFollowsTransport(followTransportButton2.getToggleState());
}

void MainComponent::selectionChanged()
{
    if (transportSource.isPlaying()) {
        showAudioResource2(juce::URL(fileTreeComp2.getSelectedFile()));
    }
    else if (transportSource2.isPlaying()) {
        showAudioResource(juce::URL(fileTreeComp.getSelectedFile()));
    }
    else {
        showAudioResource(juce::URL(fileTreeComp.getSelectedFile()));
        showAudioResource2(juce::URL(fileTreeComp2.getSelectedFile()));
    }
}       

void MainComponent::fileClicked(const juce::File&, const juce::MouseEvent&) {}
void MainComponent::fileDoubleClicked(const juce::File&) {}
void MainComponent::browserRootChanged(const juce::File&) {}
