/*
  ==============================================================================

    ThumbnailComp.h
    Created: 7 May 2021 4:36:38pm
    Author:  Matt

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ThumbnailComp : public juce::Component,
    public juce::ChangeListener,
    public juce::FileDragAndDropTarget,
    public juce::ChangeBroadcaster,
    private juce::ScrollBar::Listener,
    private juce::Timer
{
public: 

    ThumbnailComp(juce::AudioFormatManager& formatManager, juce::AudioTransportSource& source, juce::Slider& slider);

    ~ThumbnailComp() override;

    void setURL(const juce::URL& url);

    juce::URL getLastDroppedFile() const noexcept;

    void setZoomFactor(double amount);

    void setRange(juce::Range<double> newRange);

    void setFollowsTransport(bool shouldFollow);

    void paint(juce::Graphics& g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster*) override;

    bool isInterestedInFileDrag(const juce::StringArray& /*files*/) override;

    void filesDropped(const juce::StringArray& files, int /*x*/, int /*y*/) override;

    void mouseDown(const juce::MouseEvent& e) override;

    void mouseDrag(const juce::MouseEvent& e) override;

    void mouseUp(const juce::MouseEvent&) override;

    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel) override;

    //==============================================================================


private:
    juce::AudioTransportSource& transportSource;
    juce::Slider& zoomSlider;
    juce::ScrollBar scrollbar{ false };

    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail, thumbnail2;
    juce::Range<double> visibleRange;
    bool followingTransport = false;
    juce::URL lastFileDropped;

    juce::DrawableRectangle currentPositionMarker;

    float timeToX(const double time) const;

    double xToTime(const float x) const;

    bool canMoveTransport() const noexcept;

    void scrollBarMoved(juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;

    void timerCallback() override;

    void updateCursorPosition();

    //==============================================================================
};
