<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>Audio::AudioExporter</name>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="205"/>
        <source>WAV - Mixed</source>
        <translation>WAV—混合</translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="206"/>
        <source>WAV - Separated</source>
        <translation>WAV—分离</translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="207"/>
        <source>FLAC - Mixed</source>
        <translation>FLAC—混合</translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="208"/>
        <source>FLAC - Separated</source>
        <translation>FLAC—分离</translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="209"/>
        <source>Ogg/Vorbis - Mixed</source>
        <translation>Ogg/Vorbis—混合</translation>
    </message>
    <message>
        <location filename="../../audio/audioexporter.cpp" line="210"/>
        <source>Ogg/Vorbis - Separated</source>
        <translation>Ogg/Vorbis—分离</translation>
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
    <name>Audio::BuiltInFormatEntry</name>
    <message>
        <source>Option</source>
        <translation type="vanished">选项</translation>
    </message>
    <message>
        <source>Channel</source>
        <translation type="vanished">声道</translation>
    </message>
    <message>
        <source>Sample rate</source>
        <translation type="vanished">采样率</translation>
    </message>
    <message>
        <source>System</source>
        <translation type="vanished">系统</translation>
    </message>
    <message>
        <source>Little-endian</source>
        <translation type="vanished">小端序</translation>
    </message>
    <message>
        <source>Big-endian</source>
        <translation type="vanished">大端序</translation>
    </message>
    <message>
        <source>Byte order</source>
        <translation type="vanished">字节顺序</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="vanished">确定</translation>
    </message>
    <message>
        <source>Configure Raw Data</source>
        <translation type="vanished">配置原始数据</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioExportDialog</name>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="22"/>
        <source>Export Audio</source>
        <translation>导出音频</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="29"/>
        <source>Save &amp;As</source>
        <translation>另存为(&amp;A)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="31"/>
        <source>&amp;Delete</source>
        <translation>删除(&amp;D)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="33"/>
        <source>&amp;Preset</source>
        <translation>预设(&amp;P)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="43"/>
        <source>File Path</source>
        <translation>文件路径</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="45"/>
        <source>&amp;Browse</source>
        <translation>浏览(&amp;B)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="52"/>
        <source>Template</source>
        <translation>模板</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="57"/>
        <source>&lt;p&gt;You can specify the name (including extension name) of exported files.&lt;/p&gt;&lt;p&gt;Template tags in the file name will be replaced with the corresponding text. The following are the available template tags:&lt;/p&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;${projectName}&lt;/b&gt;: the base name of the project file (excluding extension name)&lt;/li&gt;&lt;li&gt;&lt;b&gt;${sampleRate}&lt;/b&gt;: the sample rate specified in export configuration&lt;/li&gt;&lt;li&gt;&lt;b&gt;${today}&lt;/b&gt;: today&apos;s date in &quot;yyyyMMdd&quot; format (e. g. &quot;19260817&quot; for August 17, 1926)&lt;/li&gt;&lt;li&gt;&lt;b&gt;${$}&lt;/b&gt;: a single &quot;$&quot; character&lt;/li&gt;&lt;/ul&gt;&lt;p&gt;In particular, the following template tags are available only when the mixing option is not &quot;mixed&quot;:&lt;/p&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;${trackName}&lt;/b&gt;: the name of track&lt;/li&gt;&lt;li&gt;&lt;b&gt;${trackIndex}&lt;/b&gt;: the index of track (starting from 1)&lt;/li&gt;&lt;/ul&gt;&lt;p&gt;You can select the template tag in the pop-up menu. The selected template tag will be appended to the end of the file name.&lt;/p&gt;</source>
        <translation>&lt;p&gt;您可以指定导出文件的名称（包括扩展名）。&lt;/p&gt;&lt;p&gt;文件名中的模板标签将替换为相应的文本。以下是可用的模板标签：&lt;/p&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;${projectName}&lt;/b&gt;：项目文件的名称（不包括扩展名）&lt;/li&gt;&lt;li&gt;&lt;b&gt;${sampleRate}&lt;/b&gt;：导出配置中指定的采样率&lt;/li&gt;&lt;li&gt;&lt;b&gt;${today}&lt;/b&gt;：今天的日期，采用“yyyyMMdd”格式（例如“19260817”表示 1926 年 8 月 17 日）&lt;/li&gt;&lt;li&gt;&lt;b&gt;${$}&lt;/b&gt;：单个“$”字符&lt;/li&gt;&lt;/ul&gt;&lt;p&gt;特别地，当混音选项不是“混合”时，可以使用以下模板标签：&lt;/p&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;${trackName}&lt;/b&gt;：音轨名称&lt;/li&gt;&lt;li&gt;&lt;b&gt;${trackIndex}&lt;/b&gt;：音轨编号（从 1 开始）&lt;/li&gt;&lt;/ul&gt;&lt;p&gt;您可以在弹出式菜单中选择模板标签。所选模板标签将附加到文件名末尾。&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="73"/>
        <source>&amp;Name</source>
        <translation>文件名(&amp;N)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="77"/>
        <source>(Project directory)</source>
        <translation>（工程目录）</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="78"/>
        <source>&amp;Directory</source>
        <translation>目录(&amp;D)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="83"/>
        <source>&amp;Type</source>
        <translation>类型(&amp;T)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="87"/>
        <source>Format</source>
        <translation>格式</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="91"/>
        <source>&amp;Option</source>
        <translation>选项(&amp;O)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="93"/>
        <source>&amp;Quality</source>
        <translation>质量(&amp;Q)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="100"/>
        <source>&amp;Sample rate</source>
        <translation>采样率(&amp;S)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="109"/>
        <source>Separated</source>
        <translation>分离</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="110"/>
        <source>Separated (through master track)</source>
        <translation>分离（通过主音轨）</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="136"/>
        <source>Loop s&amp;ection</source>
        <translation>循环区间(&amp;E)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="148"/>
        <source>&amp;Keep this dialog open after successful export</source>
        <translation>导出成功后保持此对话框打开(&amp;K)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="152"/>
        <source>Dry &amp;Run</source>
        <translation>试运行(&amp;R)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="156"/>
        <source>&quot;Dry Run&quot; shows the paths of files to export. No files will actually be exported.</source>
        <translation>“试运行”显示将要导出的文件的路径。没有文件会被实际导出。</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="163"/>
        <source>Export</source>
        <translation>导出</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="167"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>&amp;Sample Rate</source>
        <translation type="vanished">采样率(&amp;S)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="104"/>
        <source>Mixer</source>
        <translation>混音器</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="108"/>
        <source>Mixed</source>
        <translation>混合</translation>
    </message>
    <message>
        <source>Seperated</source>
        <translation type="vanished">分轨</translation>
    </message>
    <message>
        <source>Seperated (through master track)</source>
        <translation type="vanished">分轨（通过主音轨）</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="112"/>
        <source>&amp;Mixing Option</source>
        <translation>混音选项(&amp;M)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="113"/>
        <source>Enable m&amp;ute/solo</source>
        <translation>启用静音/独奏(&amp;U)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="118"/>
        <source>All tracks</source>
        <translation>所有音轨</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="119"/>
        <source>Selected tracks</source>
        <translation>选中的音轨</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="120"/>
        <source>Custom</source>
        <translation>自定义</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="122"/>
        <source>&amp;Source</source>
        <translation>源(&amp;S)</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="130"/>
        <source>Time Range</source>
        <translation>时间范围</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="133"/>
        <source>A&amp;ll</source>
        <translation>全部(&amp;L)</translation>
    </message>
    <message>
        <source>Loop &amp;interval</source>
        <translation type="vanished">循环区间(&amp;I)</translation>
    </message>
    <message>
        <source>Keep this dialog open after successful export</source>
        <translation type="vanished">导出成功后保持此对话框打开</translation>
    </message>
    <message>
        <source>Dry Run</source>
        <translation type="vanished">试运行</translation>
    </message>
    <message>
        <source>&quot;Dry Run&quot; show the paths of files to export. No files will actually be exported.</source>
        <translation type="vanished">“试运行”显示将要导出的文件的路径。没有文件会被实际导出。</translation>
    </message>
    <message>
        <location filename="../../internal/dialogs/audioexportdialog.cpp" line="161"/>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="vanished">导出(&amp;X)</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="vanished">取消(&amp;C)</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioExportPage</name>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="39"/>
        <source>Audio Export</source>
        <translation>音频导出</translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="40"/>
        <source>Configure audio export preferences.</source>
        <translation>配置音频导出首选项。</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioExportPageWidget</name>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="17"/>
        <source>Enable &amp;clipping check</source>
        <translation>启用削波检查(&amp;C)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="22"/>
        <source>Auto</source>
        <translation>自动</translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="24"/>
        <source>Audio exporting will be multi-threaded when the mixing option is &quot;separated&quot;</source>
        <translation>当混音选项为“分离”时，音频导出将使用多线程</translation>
    </message>
    <message>
        <source>Audio exporting will be multi-threaded when the mixing option is &quot;separated&quot;.</source>
        <translation type="obsolete">当混音选项为“分离”时，音频导出将使用多线程</translation>
    </message>
    <message>
        <location filename="../../internal/settings/audioexportpage.cpp" line="27"/>
        <source>Number of &amp;threads for exporting</source>
        <translation>导出所用线程数(&amp;T)</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioPage</name>
    <message>
        <location filename="../../internal/settings/audiopage.cpp" line="7"/>
        <source>Audio</source>
        <translation>音频</translation>
    </message>
    <message>
        <location filename="../../internal/settings/audiopage.cpp" line="8"/>
        <source>Configure Audio Preferences.</source>
        <translation>配置音频首选项。</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::AudioPlugin</name>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="50"/>
        <source>Initializing audio plugin...</source>
        <translation>初始化音频插件…</translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="86"/>
        <source>Cannot initialize audio output system</source>
        <translation>无法初始化音频输出系统</translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="87"/>
        <source>%1 will not play any sound because no available audio output device found. Please check the status of the audio driver and device.</source>
        <translation>由于无可用音频输出设备，%1 将不会播放声音。请检查音频驱动与设备的状态。</translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="93"/>
        <source>Cannot initialize Plugin Mode connection system</source>
        <translation>无法初始化插件模式连接系统</translation>
    </message>
    <message>
        <location filename="../../internal/audioplugin.cpp" line="94"/>
        <source>%1 will not be able to establish a connection with %1 Bridge. Please check the Plugin Mode configuration in Settings.</source>
        <translation>%1 将无法与 %1 Bridge 建立连接。请在设置中检查插件模式的配置。</translation>
    </message>
    <message>
        <source>Cannot initialize audio output system!</source>
        <translation type="vanished">无法初始化音频输出系统！</translation>
    </message>
    <message>
        <source>Cannot initialize vst connection system!</source>
        <translation type="vanished">无法初始化 VST 连接系统！</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::BuiltInFormatEntry</name>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="75"/>
        <source>Option</source>
        <translation>选项</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="79"/>
        <source>Channel</source>
        <translation>声道</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="88"/>
        <source>Sample rate</source>
        <translation>采样率</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="91"/>
        <source>System</source>
        <translation>系统</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="92"/>
        <source>Little-endian</source>
        <translation>小端序</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="93"/>
        <source>Big-endian</source>
        <translation>大端序</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="94"/>
        <source>Byte order</source>
        <translation>字节顺序</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="100"/>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <location filename="../../internal/format/builtinformatentry.cpp" line="106"/>
        <source>Configure Raw Data</source>
        <translation>配置原始数据</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::CustomizeThemeDialog</name>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="76"/>
        <source>This view demonstrates what %1 Bridge&apos;s editor looks like. May differ from actual.</source>
        <translation>此视图展示了 %1 Bridge 编辑器的外观。可能与实际有所不同。</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="86"/>
        <source>Button background</source>
        <translation>按钮背景</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="87"/>
        <source>Button border</source>
        <translation>按钮边框</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="88"/>
        <source>Button text</source>
        <translation>按钮文本</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="89"/>
        <source>Editor background</source>
        <translation>编辑器背景</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="90"/>
        <source>Bottom bar background</source>
        <translation>底栏背景</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="91"/>
        <source>Message text</source>
        <translation>消息文本</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="92"/>
        <source>Error text</source>
        <translation>错误文本</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="94"/>
        <source>Use this field to select the target of Pick Color.&lt;ul&gt;&lt;li&gt;&lt;b&gt;Button background&lt;/b&gt;: the background color of &quot;Show %1 Editor&quot; button&lt;/li&gt;&lt;li&gt;&lt;b&gt;Button border&lt;/b&gt;: the border color of &quot;Show %1 Editor&quot; button&lt;/li&gt;&lt;li&gt;&lt;b&gt;Button text&lt;/b&gt;: the color of &quot;Show %1 Editor&quot; text on the button&lt;/li&gt;&lt;li&gt;&lt;b&gt;Editor background&lt;/b&gt;: the background color of the editor&apos;s main view&lt;/li&gt;&lt;li&gt;&lt;b&gt;Bottom bar background&lt;/b&gt;: the background color of the editor&apos;s bottom bar where show the status, version and error message&lt;/li&gt;&lt;li&gt;&lt;b&gt;Message text&lt;/b&gt;: the color of the status and version text&lt;/li&gt;&lt;li&gt;&lt;b&gt;Error text&lt;/b&gt;: the color of the error message text&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="108"/>
        <source>&amp;Component</source>
        <translation>组件(&amp;C)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="109"/>
        <source>&amp;Pick Color</source>
        <translation>选择颜色(&amp;P)</translation>
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
        <translation>MIDI</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="254"/>
        <source>Configure MIDI preferences.</source>
        <translation>配置 MIDI 首选项。</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::MIDIPageWidget</name>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="45"/>
        <source>MIDI Input</source>
        <translation>MIDI 输入</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="48"/>
        <source>&amp;Device</source>
        <translation>设备(&amp;D)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="52"/>
        <source>Synthesizer</source>
        <translation>合成器</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Sine wave</source>
        <translation>正弦波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Square wave</source>
        <translation>方波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Triangle Wave</source>
        <translation>三角波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="55"/>
        <source>Sawtooth wave</source>
        <translation>锯齿波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="56"/>
        <source>&amp;Generator</source>
        <translation>生成器(&amp;G)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="68"/>
        <source>&amp;Amplitude (dB)</source>
        <translation>振幅（分贝）(&amp;A)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="81"/>
        <source>A&amp;ttack (ms)</source>
        <translation>触发时间（毫秒）(&amp;T)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="94"/>
        <source>D&amp;ecay (ms)</source>
        <translation>衰减时间（毫秒）(&amp;E)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="106"/>
        <source>Decay rati&amp;o</source>
        <translation>衰减率(&amp;O)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="119"/>
        <source>&amp;Release (ms)</source>
        <translation>释放时间（毫秒）(&amp;R)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="127"/>
        <source>Ad&amp;just by the cent shift of the active project window</source>
        <translation>根据活动工程窗口的音分偏移调整(&amp;J)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="129"/>
        <source>&amp;Frequency of A</source>
        <translation>A 的频率(&amp;F)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="134"/>
        <source>&amp;Preview</source>
        <translation>预览(&amp;P)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="137"/>
        <source>&amp;Interrupt All Notes</source>
        <translation>中断所有音符(&amp;I)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="138"/>
        <source>Interrupt all notes that are currently played by the synthesizer</source>
        <translation>中断正在被合成器播放的所有音符</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="147"/>
        <source>Control</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="155"/>
        <source>(Not working)</source>
        <translation>（不工作）</translation>
    </message>
    <message>
        <location filename="../../internal/settings/midipage.cpp" line="167"/>
        <source>Cannot open MIDI device %1</source>
        <translation>无法打开 MIDI 设备 %1</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::OutputPlaybackPage</name>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="214"/>
        <source>Output and Playback</source>
        <translation>输出与回放</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="216"/>
        <source>Specify the audio output device and playback behaviors.</source>
        <translation>指定音频输出设备与回放行为。</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::OutputPlaybackPageWidget</name>
    <message>
        <source>Output &amp; Playback</source>
        <translation type="vanished">输出与回放</translation>
    </message>
    <message>
        <source>Specify the Audio Output Device and Playback Behaviors.</source>
        <translation type="vanished">指定音频输出设备与回放行为。</translation>
    </message>
    <message>
        <source>Audio Driver &amp;&amp; Device</source>
        <translation type="vanished">音频驱动与设备</translation>
    </message>
    <message>
        <source>Audio D&amp;river</source>
        <translation type="vanished">音频驱动(&amp;R)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="63"/>
        <source>&amp;Test</source>
        <translation>测试(&amp;T)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="65"/>
        <source>Control &amp;Panel</source>
        <translation>控制面板(&amp;P)</translation>
    </message>
    <message>
        <source>Audio &amp;Device</source>
        <translation type="vanished">音频设备(&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Buffer Size</source>
        <translation type="vanished">缓冲区长度(&amp;B)</translation>
    </message>
    <message>
        <source>&amp;Sample Rate</source>
        <translation type="vanished">采样率(&amp;S)</translation>
    </message>
    <message>
        <source>Output and Playback</source>
        <translation type="vanished">输出与回放</translation>
    </message>
    <message>
        <source>Specify the audio output device and playback behaviors.</source>
        <translation type="vanished">指定音频输出设备与回放行为。</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="56"/>
        <source>Audio Driver and Device</source>
        <translation>音频驱动与设备</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="59"/>
        <source>Audio d&amp;river</source>
        <translation>音频驱动(&amp;R)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="67"/>
        <source>Audio &amp;device</source>
        <translation>音频设备(&amp;D)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="71"/>
        <source>&amp;Buffer size</source>
        <translation>缓冲区长度(&amp;B)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="73"/>
        <source>&amp;Sample rate</source>
        <translation>采样率(&amp;S)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="75"/>
        <source>Notify when any device added or removed</source>
        <translation>增加或移除任意设备时通知</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="76"/>
        <source>Notify when current device removed</source>
        <translation>移除当前设备时通知</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="77"/>
        <source>Do not notify</source>
        <translation>不通知</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="78"/>
        <source>&amp;Hot plug notification</source>
        <translation>热插拔通知(&amp;H)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="80"/>
        <source>Device &amp;Gain (dB)</source>
        <translation>设备增益（分贝）(&amp;G)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="94"/>
        <source>Device &amp;Pan</source>
        <translation>设备声像(&amp;P)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="113"/>
        <source>Return to the start position after stopped</source>
        <translation>停止后返回至开始位置</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="114"/>
        <source>Keep at current position after stopped, and play from current position next time</source>
        <translation>停止后保持在当前位置，且下次从当前位置播放</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="115"/>
        <source>Keep at current position after stopped, but play from the start position next time</source>
        <translation>停止后保持在当前位置，但下次从开始位置播放</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="117"/>
        <source>Playhead behavior</source>
        <translation>播放头行为</translation>
    </message>
    <message>
        <source>After playback is stopped, &amp;move the play head to</source>
        <translation type="vanished">回放停止后移动播放头至(&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Close audio device when playback is stopped</source>
        <translation type="vanished">回放停止时关闭音频设备(&amp;C)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="126"/>
        <source>&amp;File reading buffer size (samples)</source>
        <translation>读取文件的缓冲长度（采样）(&amp;F)</translation>
    </message>
    <message>
        <source>&amp;File reading buffer size (millisecond)</source>
        <translation type="vanished">读取文件的缓冲长度（毫秒）(&amp;F)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="138"/>
        <source>Cannot start audio playback</source>
        <translation>无法启动音频回放</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="140"/>
        <source>Please check the status of the audio driver and device.</source>
        <translation>请检查音频驱动与设备的状态。</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="280"/>
        <source>Cannot initialize %1 driver</source>
        <translation>无法初始化 %1 驱动</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="300"/>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="336"/>
        <source>Default device</source>
        <translation>默认设备</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="336"/>
        <source>Audio device %1 is not available</source>
        <translation>音频设备 %1 不可用</translation>
    </message>
    <message>
        <source>&amp;Hot Plug Notification</source>
        <translation type="vanished">热插拔通知(&amp;H)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="109"/>
        <source>Playback</source>
        <translation>回放</translation>
    </message>
    <message>
        <source>Start position</source>
        <translation type="vanished">开始位置</translation>
    </message>
    <message>
        <source>End position</source>
        <translation type="vanished">结束位置</translation>
    </message>
    <message>
        <source>After Playback is Stopped, &amp;Move the Play Head to</source>
        <translation type="vanished">回放停止后移动播放头至(&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Close Audio Device When Playback is Stopped</source>
        <translation type="vanished">回放停止时关闭音频设备(&amp;C)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="122"/>
        <source>File Caching</source>
        <translation>文件缓存</translation>
    </message>
    <message>
        <source>&amp;File Reading Buffer Size (Millisecond)</source>
        <translation type="vanished">读取文件的缓冲长度（毫秒）(&amp;F)</translation>
    </message>
    <message>
        <source>Cannot start audio playback!</source>
        <translation type="vanished">无法启动音频回放！</translation>
    </message>
    <message>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="248"/>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="285"/>
        <location filename="../../internal/settings/outputplaybackpage.cpp" line="311"/>
        <source>(Not working)</source>
        <translation>（不工作）</translation>
    </message>
    <message>
        <source>Cannot initialize %1 driver!</source>
        <translation type="vanished">无法初始化 %1 驱动！</translation>
    </message>
    <message>
        <source>Audio device %1 is not available!</source>
        <translation type="vanished">音频设备 %1 不可用！</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::PseudoSingerPage</name>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="238"/>
        <source>Pseudo Singer</source>
        <translation>伪歌手</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="239"/>
        <source>The pseudo singer is built into %1 for outputting synthesized audio. It allows you to preview a project without specifying a virtual singer.</source>
        <translation>伪歌手内置于 %1 中，用于输出合成音频，使你无需指定虚拟歌手，即可预览工程。</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::PseudoSingerPageWidget</name>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="30"/>
        <source>Synthesizer</source>
        <translation>合成器</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="38"/>
        <source>&amp;Preview</source>
        <translation>预览(&amp;P)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="41"/>
        <source>&amp;Synth No.</source>
        <translation>合成器编号(&amp;S)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Sine wave</source>
        <translation>正弦波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Square wave</source>
        <translation>方波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Triangle Wave</source>
        <translation>三角波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="50"/>
        <source>Sawtooth wave</source>
        <translation>锯齿波</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="51"/>
        <source>&amp;Generator</source>
        <translation>生成器(&amp;G)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="63"/>
        <source>&amp;Amplitude (dB)</source>
        <translation>振幅（分贝）(&amp;A)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="76"/>
        <source>A&amp;ttack (ms)</source>
        <translation>触发时间（毫秒）(&amp;T)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="89"/>
        <source>D&amp;ecay (ms)</source>
        <translation>衰减时间（毫秒）(&amp;E)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="101"/>
        <source>Decay rati&amp;o</source>
        <translation>衰减率(&amp;O)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="114"/>
        <source>&amp;Release (ms)</source>
        <translation>释放时间（毫秒）(&amp;R)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="123"/>
        <source>Parameter</source>
        <translation>参数</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="125"/>
        <source>Read pitc&amp;h</source>
        <translation>读取音高(&amp;H)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/pseudosingerpage.cpp" line="127"/>
        <source>Read energ&amp;y</source>
        <translation>读取力度(&amp;Y)</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::SettingPageSynthHelper</name>
    <message>
        <location filename="../../internal/utils/settingpagesynthhelper.cpp" line="112"/>
        <source>Why did you play ...?!</source>
        <translation>为什么要演奏………？！</translation>
    </message>
    <message>
        <location filename="../../internal/utils/settingpagesynthhelper.cpp" line="112"/>
        <source>I&apos;ll put an end to it.</source>
        <translation>我来将它结束掉。</translation>
    </message>
</context>
<context>
    <name>Audio::Internal::VSTModePage</name>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="189"/>
        <source>Plugin Mode</source>
        <translation>插件模式</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="191"/>
        <source>Plug %1 to External Host Application.</source>
        <translation>将 %1 连接到外部宿主应用程序。</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="207"/>
        <source>Connectivity</source>
        <translation>连接</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="210"/>
        <source>%1 communicates with %1 Bridge over a local TCP connection. (except for audio transmission)</source>
        <translation>%1 通过本地 TCP 连接与 %1 Bridge 通信。（除音频传输外）</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="216"/>
        <source>%1 listening &amp;port</source>
        <translation>%1 监听端口(&amp;P)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="221"/>
        <source>%1 &amp;Bridge listening port</source>
        <translation>%1 &amp;Bridge 监听端口</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="234"/>
        <source>When plugin editor is &amp;closed, make project window</source>
        <translation>当插件编辑器被关闭时，使工程窗口(&amp;C)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="236"/>
        <source>&amp;Keep project windows always on top</source>
        <translation>置顶工程窗口(&amp;K)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="240"/>
        <source>Plugin editor &amp;uses custom theme</source>
        <translation>为插件编辑器使用自定义主题(&amp;U)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="242"/>
        <source>Customize &amp;Theme</source>
        <translation>自定义主题(&amp;T)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="257"/>
        <source>Test Audio</source>
        <translation>测试音频</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="291"/>
        <source>Host</source>
        <translation>宿主</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="292"/>
        <source>Plugin Format</source>
        <translation>插件格式</translation>
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
        <translation>不可用</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="300"/>
        <source>Buffer Size</source>
        <translation>缓冲区长度</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="307"/>
        <source>Sample Rate</source>
        <translation>采样率</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="314"/>
        <source>Connected</source>
        <translation>已连接</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="317"/>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="319"/>
        <source>No</source>
        <translation>否</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="332"/>
        <source>Property</source>
        <translation>属性</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="332"/>
        <source>Value</source>
        <translation>值</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="340"/>
        <source>Cannot start audio playback</source>
        <translation>无法启动音频回放</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="341"/>
        <source>Please check the status of %1 Bridge and the host application.</source>
        <translation>请检查 %1 Bridge 和宿主应用程序的状态。</translation>
    </message>
    <message>
        <source>%1 Listening &amp;Port</source>
        <translation type="vanished">%1 监听端口(&amp;P)</translation>
    </message>
    <message>
        <source>%1 &amp;Bridge Listening Port</source>
        <translation type="vanished">%1 &amp;Bridge 监听端口</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="224"/>
        <source>Only after both %1 and %1 Bridge are restarted will these options take effect.</source>
        <translation>只有在 %1 和 %1 Bridge 都重新启动之后这些选项才会生效。</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="230"/>
        <source>Editor</source>
        <translation>编辑器</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="233"/>
        <source>Hide</source>
        <translation>隐藏</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="233"/>
        <source>Minimize</source>
        <translation>最小化</translation>
    </message>
    <message>
        <source>When Plugin Editor is &amp;Closed, Make Project Window</source>
        <translation type="vanished">当插件编辑器被关闭时，使工程窗口(&amp;C)</translation>
    </message>
    <message>
        <location filename="../../internal/settings/vstmodepage.cpp" line="252"/>
        <source>Status</source>
        <translation>状态</translation>
    </message>
</context>
</TS>
