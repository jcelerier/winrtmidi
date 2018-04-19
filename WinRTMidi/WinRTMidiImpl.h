// ******************************************************************
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THE CODE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
// THE CODE OR THE USE OR OTHER DEALINGS IN THE CODE.
// ******************************************************************

#pragma once

#include "WinRTMidi.h"
#include "WinRTMidiPortWatcher.h"
#include <memory>
#include <string>
#include <Windows.h>

namespace WinRT
{
    ref class WinRTMidiPort abstract
    {
    public:
        virtual ~WinRTMidiPort();
        virtual void ClosePort(void) = 0;

    internal:
        WinRTMidiPort();
        virtual WinRTMidiErrorType OpenPort(Platform::String^ id) = 0;

        void SetError(WinRTMidiErrorType error, const std::string& message);
        const std::string& GetErrorMessage();
        WinRTMidiErrorType GetError();

    private:
        std::string mErrorMessage;
        WinRTMidiErrorType mError;
    };

    ref class WinRTMidiInPort sealed : public WinRTMidiPort
    {
    public:
        virtual ~WinRTMidiInPort();

        virtual void ClosePort(void) override;

        void RemoveMidiInCallback() {
            mMessageReceivedCallback = nullptr;
        };

    internal:
        WinRTMidiInPort();
        virtual WinRTMidiErrorType OpenPort(Platform::String^ id) override;

        // needs to be internal as MidiInMessageReceivedCallbackType is not a WinRT type
        void SetMidiInCallback(void* ctx, WinRTMidiInCallback callback) {
            mCtx = ctx;
            mMessageReceivedCallback = callback;
        };

    private:
        void OnMidiInMessageReceived(Windows::Devices::Midi::MidiInPort^ sender, Windows::Devices::Midi::MidiMessageReceivedEventArgs^ args);
        Windows::Devices::Midi::MidiInPort^ mMidiInPort;
        long long mLastMessageTime;
        bool mFirstMessage;
        void* mCtx{};
        WinRTMidiInCallback mMessageReceivedCallback;
    };

    ref class WinRTMidiOutPort sealed : public WinRTMidiPort
    {
    public:
        virtual ~WinRTMidiOutPort();

        virtual void ClosePort(void) override;

    internal:
        WinRTMidiOutPort();
        virtual WinRTMidiErrorType OpenPort(Platform::String^ id) override;
        void Send(const unsigned char* message, unsigned int nBytes);

    private:
        byte* getIBufferDataPtr(Windows::Storage::Streams::IBuffer^ buffer);

        Windows::Devices::Midi::IMidiOutPort^ mMidiOutPort;
        Windows::Storage::Streams::IBuffer^ mBuffer;
        byte* mBufferData;
    };

    class WinRTMidi
    {
    public:
        WinRTMidi(void* ctx, MidiPortChangedCallback callback);
        WinRTMidiErrorType Initialize();

        WinRTMidiPortWatcher^ GetPortWatcher(WinRTMidiPortType type);
        WinRTMidiPortWatcherPtr GetPortWatcherWrapper(WinRTMidiPortType type);
        Platform::String^ getPortId(WinRTMidiPortType type, unsigned int index);

    private:
        WinRTMidiPortWatcher^ mMidiInPortWatcher;
        WinRTMidiPortWatcher^ mMidiOutPortWatcher;
        std::shared_ptr<MidiPortWatcherWrapper> mMidiInPortWatcherWrapper;
        std::shared_ptr<MidiPortWatcherWrapper> mMidiOutPortWatcherWrapper;
    };

    class MidiInPortWrapper
    {
    public:
        MidiInPortWrapper(WinRTMidiInPort^ port, void* ctx, WinRTMidiInCallback callback)
            : mPort(port)
        {
            mPort->SetMidiInCallback(ctx, callback);
        }

        WinRTMidiInPort^ getPort() { return mPort; };

    private:
        WinRTMidiInPort^ mPort;
    };

    class MidiOutPortWrapper
    {
    public:
        MidiOutPortWrapper(WinRTMidiOutPort^ port)
            : mPort(port)
        {}

        WinRTMidiOutPort^ getPort() { return mPort; };

    private:
        WinRTMidiOutPort^ mPort;
    };
};

