<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW">
<context>
    <name>Audio::AudioExporter</name>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="205"/>
        <source>WAV - Mixed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="206"/>
        <source>WAV - Separated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="207"/>
        <source>FLAC - Mixed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="208"/>
        <source>FLAC - Separated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="209"/>
        <source>Ogg/Vorbis - Mixed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="210"/>
        <source>Ogg/Vorbis - Separated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="257"/>
        <source>No file will be exported. Please check if any source is selected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="260"/>
        <source>The files to be exported contain files with duplicate names. Please check if the file name template is unique for each source.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="263"/>
        <source>The files to be exported contain files with the same name as existing files.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="266"/>
        <source>Unrecognized file name template. Please check the syntax of the file name template.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="269"/>
        <source>Currently selected file type is a lossy format. Please use WAV or FLAC format to avoid loss of sound quality.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioExportDialog</name>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="22"/>
        <source>Export Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="29"/>
        <source>Save &amp;As</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="31"/>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="33"/>
        <source>&amp;Preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="43"/>
        <source>File Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="45"/>
        <source>&amp;Browse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="52"/>
        <source>Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="57"/>
        <source>&lt;p&gt;You can specify the name (including extension name) of exported files.&lt;/p&gt;&lt;p&gt;Template tags in the file name will be replaced with the corresponding text. The following are the available template tags:&lt;/p&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;${projectName}&lt;/b&gt;: the base name of the project file (excluding extension name)&lt;/li&gt;&lt;li&gt;&lt;b&gt;${sampleRate}&lt;/b&gt;: the sample rate specified in export configuration&lt;/li&gt;&lt;li&gt;&lt;b&gt;${today}&lt;/b&gt;: today&apos;s date in &quot;yyyyMMdd&quot; format (e. g. &quot;19260817&quot; for August 17, 1926)&lt;/li&gt;&lt;li&gt;&lt;b&gt;${$}&lt;/b&gt;: a single &quot;$&quot; character&lt;/li&gt;&lt;/ul&gt;&lt;p&gt;In particular, the following template tags are available only when the mixing option is not &quot;mixed&quot;:&lt;/p&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;${trackName}&lt;/b&gt;: the name of track&lt;/li&gt;&lt;li&gt;&lt;b&gt;${trackIndex}&lt;/b&gt;: the index of track (starting from 1)&lt;/li&gt;&lt;/ul&gt;&lt;p&gt;You can select the template tag in the pop-up menu. The selected template tag will be appended to the end of the file name.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="73"/>
        <source>&amp;Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="77"/>
        <source>(Project directory)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="78"/>
        <source>&amp;Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="83"/>
        <source>&amp;Type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="87"/>
        <source>Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="91"/>
        <source>&amp;Option</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="93"/>
        <source>&amp;Quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="100"/>
        <source>&amp;Sample rate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="104"/>
        <source>Mixer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="108"/>
        <source>Mixed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="109"/>
        <source>Separated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="110"/>
        <source>Separated (through master track)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="112"/>
        <source>&amp;Mixing Option</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="113"/>
        <source>Enable m&amp;ute/solo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="118"/>
        <source>All tracks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="119"/>
        <source>Selected tracks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="120"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="122"/>
        <source>&amp;Source</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="130"/>
        <source>Time Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="133"/>
        <source>A&amp;ll</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="136"/>
        <source>Loop s&amp;ection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="148"/>
        <source>&amp;Keep this dialog open after successful export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="152"/>
        <source>Dry &amp;Run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="156"/>
        <source>&quot;Dry Run&quot; shows the paths of files to export. No files will actually be exported.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="161"/>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="163"/>
        <source>Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="167"/>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioExportPage</name>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="39"/>
        <source>Audio Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="40"/>
        <source>Configure audio export preferences.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioExportPageWidget</name>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="17"/>
        <source>Enable &amp;clipping check</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="22"/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="24"/>
        <source>Audio exporting will be multi-threaded when the mixing option is &quot;separated&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="27"/>
        <source>Number of &amp;threads for exporting</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioPage</name>
    <message>
        <location filename="../../internal/settings/audiopage.cpp" line="7"/>
        <source>Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/audiopage.cpp" line="8"/>
        <source>Configure Audio Preferences.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioPlugin</name>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="50"/>
        <source>Initializing audio plugin...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="86"/>
        <source>Cannot initialize audio output system</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="87"/>
        <source>%1 will not play any sound because no available audio output device found. Please check the status of the audio driver and device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="93"/>
        <source>Cannot initialize Plugin Mode connection system</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="94"/>
        <source>%1 will not be able to establish a connection with %1 Bridge. Please check the Plugin Mode configuration in Settings.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::BuiltInFormatEntry</name>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="75"/>
        <source>Option</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="79"/>
        <source>Channel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="88"/>
        <source>Sample rate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="91"/>
        <source>System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="92"/>
        <source>Little-endian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="93"/>
        <source>Big-endian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="94"/>
        <source>Byte order</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="100"/>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="106"/>
        <source>Configure Raw Data</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::CustomizeThemeDialog</name>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="76"/>
        <source>This view demonstrates what %1 Bridge&apos;s editor looks like. May differ from actual.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="86"/>
        <source>Button background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="87"/>
        <source>Button border</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="88"/>
        <source>Button text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="89"/>
        <source>Editor background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="90"/>
        <source>Bottom bar background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="91"/>
        <source>Message text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="92"/>
        <source>Error text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="94"/>
        <source>Use this field to select the target of Pick Color.&lt;ul&gt;&lt;li&gt;&lt;b&gt;Button background&lt;/b&gt;: the background color of &quot;Show %1 Editor&quot; button&lt;/li&gt;&lt;li&gt;&lt;b&gt;Button border&lt;/b&gt;: the border color of &quot;Show %1 Editor&quot; button&lt;/li&gt;&lt;li&gt;&lt;b&gt;Button text&lt;/b&gt;: the color of &quot;Show %1 Editor&quot; text on the button&lt;/li&gt;&lt;li&gt;&lt;b&gt;Editor background&lt;/b&gt;: the background color of the editor&apos;s main view&lt;/li&gt;&lt;li&gt;&lt;b&gt;Bottom bar background&lt;/b&gt;: the background color of the editor&apos;s bottom bar where show the status, version and error message&lt;/li&gt;&lt;li&gt;&lt;b&gt;Message text&lt;/b&gt;: the color of the status and version text&lt;/li&gt;&lt;li&gt;&lt;b&gt;Error text&lt;/b&gt;: the color of the error message text&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="108"/>
        <source>&amp;Component</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="109"/>
        <source>&amp;Pick Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="111"/>
        <source>After pressing this button, you can set a custom color for the selected target in the pop-up dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="120"/>
        <source>Customize Theme</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::MIDIPage</name>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="253"/>
        <source>MIDI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="254"/>
        <source>Configure MIDI preferences.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::MIDIPageWidget</name>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="45"/>
        <source>MIDI Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="48"/>
        <source>&amp;Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="52"/>
        <source>Synthesizer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Sine wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Square wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Triangle Wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Sawtooth wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="56"/>
        <source>&amp;Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="68"/>
        <source>&amp;Amplitude (dB)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="81"/>
        <source>A&amp;ttack (ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="94"/>
        <source>D&amp;ecay (ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="106"/>
        <source>Decay rati&amp;o</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="119"/>
        <source>&amp;Release (ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="127"/>
        <source>Ad&amp;just by the cent shift of the active project window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="129"/>
        <source>&amp;Frequency of A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="134"/>
        <source>&amp;Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="137"/>
        <source>&amp;Interrupt All Notes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="138"/>
        <source>Interrupt all notes that are currently played by the synthesizer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="147"/>
        <source>Control</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="155"/>
        <source>(Not working)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="167"/>
        <source>Cannot open MIDI device %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::OutputPlaybackPage</name>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="214"/>
        <source>Output and Playback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="216"/>
        <source>Specify the audio output device and playback behaviors.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::OutputPlaybackPageWidget</name>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="56"/>
        <source>Audio Driver and Device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="59"/>
        <source>Audio d&amp;river</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="63"/>
        <source>&amp;Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="65"/>
        <source>Control &amp;Panel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="67"/>
        <source>Audio &amp;device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="71"/>
        <source>&amp;Buffer size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="73"/>
        <source>&amp;Sample rate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="75"/>
        <source>Notify when any device added or removed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="76"/>
        <source>Notify when current device removed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="77"/>
        <source>Do not notify</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="78"/>
        <source>&amp;Hot plug notification</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="80"/>
        <source>Device &amp;Gain (dB)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="94"/>
        <source>Device &amp;Pan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="109"/>
        <source>Playback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="113"/>
        <source>Return to the start position after stopped</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="114"/>
        <source>Keep at current position after stopped, and play from current position next time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="115"/>
        <source>Keep at current position after stopped, but play from the start position next time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="117"/>
        <source>Playhead behavior</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="122"/>
        <source>File Caching</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="126"/>
        <source>&amp;File reading buffer size (samples)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="138"/>
        <source>Cannot start audio playback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="140"/>
        <source>Please check the status of the audio driver and device.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="248"/>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="285"/>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="311"/>
        <source>(Not working)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="280"/>
        <source>Cannot initialize %1 driver</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="300"/>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="336"/>
        <source>Default device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="336"/>
        <source>Audio device %1 is not available</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::PseudoSingerPage</name>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="238"/>
        <source>Pseudo Singer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="239"/>
        <source>The pseudo singer is built into %1 for outputting synthesized audio. It allows you to preview a project without specifying a virtual singer.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::PseudoSingerPageWidget</name>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="30"/>
        <source>Synthesizer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="38"/>
        <source>&amp;Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="41"/>
        <source>&amp;Synth No.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Sine wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Square wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Triangle Wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Sawtooth wave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="51"/>
        <source>&amp;Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="63"/>
        <source>&amp;Amplitude (dB)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="76"/>
        <source>A&amp;ttack (ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="89"/>
        <source>D&amp;ecay (ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="101"/>
        <source>Decay rati&amp;o</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="114"/>
        <source>&amp;Release (ms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="123"/>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="125"/>
        <source>Read pitc&amp;h</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="127"/>
        <source>Read energ&amp;y</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::SettingPageSynthHelper</name>
    <message>
        <location filename="../../internal/utils/settingpagesynthhelper.cpp" line="112"/>
        <source>Why did you play ...?!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/utils/settingpagesynthhelper.cpp" line="112"/>
        <source>I&apos;ll put an end to it.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Audio::Internal::VSTModePage</name>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="189"/>
        <source>Plugin Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="191"/>
        <source>Plug %1 to External Host Application.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="207"/>
        <source>Connectivity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="210"/>
        <source>%1 communicates with %1 Bridge over a local TCP connection. (except for audio transmission)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="216"/>
        <source>%1 listening &amp;port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="221"/>
        <source>%1 &amp;Bridge listening port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="224"/>
        <source>Only after both %1 and %1 Bridge are restarted will these options take effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="230"/>
        <source>Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="233"/>
        <source>Hide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="233"/>
        <source>Minimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="234"/>
        <source>When plugin editor is &amp;closed, make project window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="236"/>
        <source>&amp;Keep project windows always on top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="240"/>
        <source>Plugin editor &amp;uses custom theme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="242"/>
        <source>Customize &amp;Theme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="252"/>
        <source>Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="257"/>
        <source>Test Audio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="291"/>
        <source>Host</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="292"/>
        <source>Plugin Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="294"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="295"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="302"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="309"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="320"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="321"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="322"/>
        <location filename="../../internal/settings/vstmodepage.cpp" line="323"/>
        <source>N/A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="300"/>
        <source>Buffer Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="307"/>
        <source>Sample Rate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="314"/>
        <source>Connected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="317"/>
        <source>Yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="319"/>
        <source>No</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="332"/>
        <source>Property</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="332"/>
        <source>Value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="340"/>
        <source>Cannot start audio playback</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="341"/>
        <source>Please check the status of %1 Bridge and the host application.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
