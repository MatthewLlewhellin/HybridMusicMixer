/*
  ==============================================================================

    ThumbnailComp.cpp
    Created: 7 May 2021 4:36:38pm
    Author:  Matt

  ==============================================================================
*/

#include "ThumbnailComp.h"

ThumbnailComp::ThumbnailComp(juce::AudioFormatManager& formatManager,
    juce::AudioTransportSource& source,
    juce::Slider& slider) : transportSource(source),
                            zoomSlider(slider),
                             thumbnail(512, formatManager, thumbnailCache),
                            thumbnail2(512, formatManager, thumbnailCache)
{

    thumbnail.addChangeListener(this);
    thumbnail2.addChangeListener(this);

    addAndMakeVisible(scrollbar);

    scrollbar.setRangeLimits(visibleRange);
    scrollbar.setAutoHide(false);
    scrollbar.addListener(this);

    currentPositionMarker.setFill(juce::Colours::white.withAlpha(0.85f));
    addAndMakeVisible(currentPositionMarker);
}

ThumbnailComp::~ThumbnailComp(){
    scrollbar.removeListener(this);
    thumbnail.removeChangeListener(this);

    thumbnail2.removeChangeListener(this);
}

void ThumbnailComp::setURL(const juce::URL& url) {
    juce::InputSource* inputSource = nullptr;
    
    if (url.isLocalFile()) {
        inputSource = new juce::FileInputSource(url.getLocalFile());
    }
    if (inputSource != nullptr) {
        thumbnail.setSource(inputSource);

        juce::Range<double> newRange(0.0, thumbnail.getTotalLength());
        scrollbar.setRangeLimits(newRange);
        setRange(newRange);

        startTimerHz(40);
    }
}

juce::URL ThumbnailComp::getLastDroppedFile() const noexcept { return lastFileDropped; }

void ThumbnailComp::setZoomFactor(double amount) {
    if (thumbnail.getTotalLength() > 0) {
        auto newScale = juce::jmax(0.001, thumbnail.getTotalLength() * (1.0 - juce::jlimit(0.0, 0.99, amount)));
        auto timeAtCentre = xToTime((float)getWidth() / 2.0f);
        setRange({ timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5 });
    }
}

void ThumbnailComp::setRange(juce::Range<double> newRange)
{
    visibleRange = newRange;
    scrollbar.setCurrentRange(visibleRange);
    updateCursorPosition();
    repaint();
}

void ThumbnailComp::setFollowsTransport(bool shouldFollow)
{
    followingTransport = shouldFollow;
}

void ThumbnailComp::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::lightblue);

    if (thumbnail.getTotalLength() > 0.0) {
        auto thumbArea = getLocalBounds();
        thumbArea.removeFromBottom(scrollbar.getHeight() + 4);
        thumbnail.drawChannels(g, thumbArea.reduced(2), visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
    }
    else {
        g.setFont(14.0f);
        g.drawFittedText("(No audio file selected)", getLocalBounds(), juce::Justification::centred, 2);
    }
}

void ThumbnailComp::resized() {
    scrollbar.setBounds(getLocalBounds().removeFromBottom(14).reduced(2));
}

void ThumbnailComp::changeListenerCallback(ChangeBroadcaster*) {
    repaint();
}

bool ThumbnailComp::isInterestedInFileDrag(const juce::StringArray&/*files*/) {
    return true;
}

void ThumbnailComp::filesDropped(const juce::StringArray& files, int/*x*/, int/*y*/) {
    lastFileDropped = juce::URL(juce::File(files[0]));
    sendChangeMessage();
}

void ThumbnailComp::mouseDown(const juce::MouseEvent& e)
{
    mouseDrag(e);
}

void ThumbnailComp::mouseDrag(const juce::MouseEvent& e)
{
    if (canMoveTransport())
        transportSource.setPosition(juce::jmax(0.0, xToTime((float)e.x)));
}

void ThumbnailComp::mouseUp(const juce::MouseEvent&)
{
    transportSource.start();
}

void ThumbnailComp::mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)
{
    if (thumbnail.getTotalLength() > 0.0)
    {
        auto newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
        newStart = juce::jlimit(0.0, juce::jmax(0.0, thumbnail.getTotalLength() - (visibleRange.getLength())), newStart);

        if (canMoveTransport())
            setRange({ newStart, newStart + visibleRange.getLength() });

        if (wheel.deltaY != 0.0f)
            zoomSlider.setValue(zoomSlider.getValue() - wheel.deltaY);

        repaint();
    }
}

float ThumbnailComp::timeToX(const double time) const
{
    if (visibleRange.getLength() <= 0)
        return 0;

    return (float)getWidth() * (float)((time - visibleRange.getStart()) / visibleRange.getLength());
}

double ThumbnailComp::xToTime(const float x) const
{
    return (x / (float)getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
}

bool ThumbnailComp::canMoveTransport() const noexcept
{
    return !(followingTransport && transportSource.isPlaying());
}

void ThumbnailComp::scrollBarMoved(juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    if (scrollBarThatHasMoved == &scrollbar)
        if (!(followingTransport && transportSource.isPlaying()))
            setRange(visibleRange.movedToStartAt(newRangeStart));
}

void ThumbnailComp::timerCallback()
{
    if (canMoveTransport())
        updateCursorPosition();
    else
        setRange(visibleRange.movedToStartAt(transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
}

void ThumbnailComp::updateCursorPosition()
{
    currentPositionMarker.setVisible(transportSource.isPlaying() || isMouseButtonDown());

    currentPositionMarker.setRectangle(juce::Rectangle<float>(timeToX(transportSource.getCurrentPosition()) - 0.75f, 0,
        1.5f, (float)(getHeight() - scrollbar.getHeight())));
}
//==============================================================================