   M S B A M L   `   `   `  ���� O  KRealtimeImageControl, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null;"clr-namespace:RealtimeImageControlRealtimeImageControl  ,    &RealtimeImageControl.ModelPanelControl   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     @d2http://schemas.microsoft.com/expression/blend/2008     4local"clr-namespace:RealtimeImageControl     -     >�Name$  userControl�� � 	Resources  �� MergedDictionaries �� Source��  �� $B </RealtimeImageControl;component/Style/ModelControlStyle.xaml?�%    .��� $��
18,0,16,15q�.+�� -    �Name$ mainGrid�� � RowDefinitions �� $��46 ��� $��* �.+�]� ��.  .+��� -    ��Name$ BtnNewModel��$��94��$��24��.����  �� Orientation$ 
Horizontal=�.+��� $��Center=�$��0q�$��20��$��20������  �� 
TargetType" ��.��� 	 �� Property	 � ��
 �� Value$K
 E/RealtimeImageControl;component/Resources/btn_新建模型_normal.png�� �� Triggers ��  �� Binding ��  �� ElementName$ BtnNewModel�� �� Path$ 	IsPressed� �� Value True.a��� 	 � ��$J
 D/RealtimeImageControl;component/Resources/btn_新建模型_click.png����  �� $ BtnNewModel��$ IsMouseOver� True.a��� 	 � ��$J
 D/RealtimeImageControl;component/Resources/btn_新建模型_click.png����  �� $ BtnNewModel��$ 	IsEnabled�	 False.a��� 	 � ��$L
 F/RealtimeImageControl;component/Resources/btn_新建模型_disable.png���� $��Center=�$��新建模型������ " ��   ButtonStyle �� BasedOn# [   ��  �� *
ModelExist+$ userControl�� True.a��� 	 � y�$
 	Collapsed=��� -   $ BtnEditModel��#��[  $��5,0,0,0q�$��94��$��24��y��� *
ModelExist+$ userControl��  bool2VisibilityConverter �� 	Converter# [ .���� $ 
Horizontal=�.+��� $��Center=�$��0q�$��20��$��20������ " ��.��� 	 � ��$K
 E/RealtimeImageControl;component/Resources/btn_编辑模型_normal.png�� ��  �� $ BtnEditModel��$ 	IsPressed� True.a��� 	 � ��$J
 D/RealtimeImageControl;component/Resources/btn_编辑模型_click.png����  �� $ BtnEditModel��$ IsMouseOver� True.a��� 	 � ��$J
 D/RealtimeImageControl;component/Resources/btn_编辑模型_click.png����  �� $ BtnEditModel��$ 	IsEnabled�	 False.a��� 	 � ��$L
 F/RealtimeImageControl;component/Resources/btn_编辑模型_disable.png���� $��Center=�$��编辑模型���� -    � ToolTip$ 导出��$��Right=�$��34��$��34��{��� *
ModelExist+$ userControl������  �� 
TargetType" ��.R���  ��Name$ ImageBackground��$��34��$��34��$E��?/RealtimeImageControl;component/Resources/btn_导出_normal.png�� �� Triggers P�  P� Property �@J�IsMouseOver P� Value . . ���  �� 
TargetName$ ImageBackground��	 � ��$D
 >/RealtimeImageControl;component/Resources/btn_导出_click.png��P�  � �� . . ��� $ ImageBackground��	 � ��$D
 >/RealtimeImageControl;component/Resources/btn_导出_click.png��P�  � {� .  . ��� $ ImageBackground��	 � ��$F
 @/RealtimeImageControl;component/Resources/btn_导出_disable.png���� -   $ 导入��$��Right=�$��34��$��34������ " ��.R��� $ ImageBackground��$��34��$��34��$E��?/RealtimeImageControl;component/Resources/btn_导入_normal.png�� P�  �@J�IsMouseOver . . ��� $ ImageBackground��	 � ��$D
 >/RealtimeImageControl;component/Resources/btn_导入_click.png��P�  � �� . . ��� $ ImageBackground��	 � ��$D
 >/RealtimeImageControl;component/Resources/btn_导入_click.png��P�  � {� .  . ��� $ ImageBackground��	 � ��$G
 A/RealtimeImageControl;component/Resources/btn_导入_disabkle.png���� $��1�����#FCFCFC���#E0DFDE$��1q�._��� $��Center=�.+��� -   $ imgModel��$
��None=�$��Center=�$=��7/RealtimeImageControl;component/Resources/空模型.png��