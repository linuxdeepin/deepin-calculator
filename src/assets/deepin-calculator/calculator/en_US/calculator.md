# Calculator|deepin-calculator|

## Overview

Calculator is a simple and easy-to-use desktop calculator. It supports standard mode, scientific mode and programmer mode, with keyboard input perfectly matched, as well as symbolic fault-tolerant computing and calculation result linkage. The current version is 5.8.24.4. This manual is written against the real interface and reviewed behavior of that version, and refines the original system manual with high-reuse incremental calibration.

> ![icon](../common/notes.svg)Notes: Functions marked as "verified" in this document were actually performed in Calculator 5.8.24.4 during this verification round and produced reviewable results. Features whose entries are visible but were not exercised one by one are explicitly marked as "not verified item by item" and are not presented as verified conclusions. Memory, Help and Exit reuse the original system manual and were not fully closed in this round.

## Modes

Click ![icon_menu](../common/icon_menu.svg) > **Mode** to:

- Select **Standard** to perform the four fundamental operations of arithmetic;
- Select **Scientific** to perform high-level operations such as function, exponent, root, and so on;
- Select **Programmer** to perform binary, octal, decimal, hexadecimal and other complex operations.

The Mode submenu contains Standard, Scientific and Programmer, with a check mark on the current item. Switching among the three modes was actually verified. The window resizes with the mode: Standard about 430x681, Scientific about 564x678, and Programmer about 564x718.

> Note: no English figure was captured for the Mode menu in this round, and no reused English figure is available for it; the Chinese edition keeps the verified menu figure.

### Standard Mode

Standard Mode provides the four basic operations, percent, brackets, memory and history.

![0|standard](fig/standard_mode.png)

| Icon | Name | Description |
| --- | --- | --- |
| 0~9 | Number Key | Basic Arabic numerals. |
| MC | Clear Key | Clear all memories (description reused from the original system manual; not exercised item by item in this round). |
| MR | Storage Key | Memory recall (reused from the original system manual; not exercised item by item in this round). |
| M+ | Storage Key | Memory add; click it to add the current number accumulatively to the memory and interrupt digital input (reused from the original system manual; not exercised item by item in this round). |
| M- | Storage Key | Memory subtract; click it to subtract the current number from the memory and interrupt digital input (reused from the original system manual; not exercised item by item in this round). |
| MS | Storage Key | Memory store; click it to add the numeric value in the input box to the memory list (reused from the original system manual; not exercised item by item in this round). |
| ![M](../common/M.png) | Storage Key | Click ![M](../common/M.png) to expand the memory list; click again to fold it. The memory is cleared when Calculator is closed. The current memory key label is **M^v**; the **MH** label in the original system manual is no longer used. Its function description is reused from the original manual and was not exercised item by item in this round. |
| C | Clear | Clear the current expression. The current Standard Mode interface shows a single **C** key. |
| % | Percent Sign | To input percent sign. |
| ![delete](../common/delete.svg) | Delete | Click once to delete one character forward. |
| +-x/ | Addition, subtraction, multiplication, and division | Basic math operators for addition, subtraction, multiplication and division. |
| . | Decimal Point | To input decimal point. |
| () | Bracket | To input brackets with the left and right bracket completed automatically. If you type from the keyboard, an opening bracket produces an opening bracket and a closing bracket produces a closing bracket; if only one side appears, the expression is incorrect. |
| = | Equal Sign | To get the result. |

> Note: the reused English figure for Standard Mode comes from the original English manual and was not re-captured for the current interface in this round. Its labels differ from the current 5.8.24.4 interface in one place: the third key of the bottom row is shown as `0` in the old figure while the current interface shows `()`.

### Scientific Mode

Scientific Mode adds trigonometric functions, logarithms, powers, roots, constants and angle unit switching to the standard operations.

![0|scientific](fig/scientific_mode.png)

| Icon | Name | Description |
| --- | --- | --- |
| F-E | E-notation | Click F-E to start E-notation; click again to close it. |
| ![icon](../common/MH.svg) | Storage Key | The current Scientific Mode memory key label is **M/H^v**; click it to show the memory list and history (reused from the original system manual; not exercised item by item in this round). |
| ![icon](fig/deg.png) | Angle Unit | The current Scientific interface provides **deg** (degree) and **rad** (radian) switching. Only deg was verified in this round; rad was not exercised. The **grad** (gradian) option in the original system manual is no longer listed separately in the current interface. |
| sin, cos, tan, cot | Trigonometric functions | Calculate the sine, cosine, tangent and cotangent of the value respectively. In this round, sin(30) = 0.5 was verified under deg. |
| sin<sup>-1</sup>, cos<sup>-1</sup>, tan<sup>-1</sup>, cot<sup>-1</sup> | Anti-trigonometric functions | Click 2<sup>nd</sup> to switch to the second function interface and calculate the anti-trigonometric functions of sin, cos, tan and cot respectively; the entries are visible but were not exercised item by item in this round. |
| &#124;x&#124;, Rand | F functions | Calculate the absolute value of the value and randomly display a 31-digit number. |
| 2<sup>nd</sup> | 2<sup>nd</sup> function key | Click 2<sup>nd</sup> to switch to the second function interface; click again to return to the trigonometric functions and power operations. |
| x<sup>2</sup>, x<sup>3</sup>, x<sup>y</sup> | Power function | Calculate the square, cube and y-power of the value respectively. |
| 10<sup>x</sup>, 2<sup>x</sup>, e<sup>x</sup> | Exponential function | Calculate the x-power of 10, of 2 and of e respectively; 2<sup>x</sup> and e<sup>x</sup> are second function interface buttons. In this round, 2^10 = 1,024 was verified. |
| ![icon](fig/square_root.png), ![icon](fig/cube_root.png), ![icon](fig/y_root.png) | Root function | Click 2<sup>nd</sup> to switch to the second function interface and calculate the square root, cube root and y-th root of x respectively; the entries are visible but were not exercised item by item in this round. |
| log, ln, log<sub>y</sub><sup>x</sup> | Logarithmic function | Calculate the logarithm based on 10, the logarithm based on e, and the logarithm of x to base y respectively; log<sub>y</sub><sup>x</sup> is a second function interface button. |
| pi | PI | Approximately 3.14159..., accurate to 31 digits after the decimal point. |
| e | Constant | Approximately 2.71828..., accurate to 31 digits after the decimal point. |
| Mod | Remainder | Display the modulus or remainder of x / y. |
| 1/x | Inverse proportional function | Calculate the reciprocal of the displayed value. |
| x! | Factorial | Calculate the factorial of the displayed number. |
| exp | Exponent | Enter numbers in scientific notation. |

> ![icon](../common/notes.svg)Notes: The current Scientific interface provides deg/rad. In this round only sin(30) = 0.5 under deg and 2^10 = 1,024 were verified. Switching to rad, the anti-trigonometric functions, the root functions and the logarithmic functions were not exercised item by item and must not be presented as verified.

### Programmer Mode

Programmer Mode shows HEX, DEC, OCT and BIN with synchronized values on the left, and provides logical operations and shift entries.

![0|programmer](fig/programmer.png)

| Icon | Name | Description |
| --- | --- | --- |
| HEX, DEC, OCT, BIN | Hex | Hexadecimal, decimal, octal and binary respectively; decimal is the default. In this round the four bases were verified to stay in sync: DEC 255 = HEX FF = OCT 377 = BIN 1111 1111. |
| ![icon](../common/back.svg) | Full keyboard | Click it to return to the full keyboard interface. |
| ![icon](../common/bit.svg) | Digit switching keyboard | Show 0~63 digit bits; clicking each bit is supported. The entry is visible but was not expanded and verified bit by bit in this round. |
| QWORD/DWORD/WORD/BYTE | Data type | Click a button to select the mode: Quadword (64 bits), DWord (32 bits), Word (16 bits) and Byte (8 bits). The current default is QWORD; each type was not switched and verified one by one in this round. |
| ![icon](../common/arithmetic.svg)/![icon](../common/logical.svg)/![icon](../common/circular.svg)/![icon](../common/rotate.svg) | Bit shifting | Arithmetic shift, logical shift, circular shift and rotate through carry circular shift respectively; the entries are visible but were not exercised item by item in this round. |
| AND, OR, NOT, NAND, NOR, XOR | Logical operators | AND, OR, NOT, NAND, NOR and XOR respectively; the entries are visible but were not exercised item by item in this round. |
| A~F | Letters | Only activated in hexadecimal. |
| <<, >> | Movement operators | Move left or right respectively; the entries are visible but were not exercised item by item in this round. |

> ![icon](../common/notes.svg)Notes: Base conversion (DEC 255 synchronized with HEX/OCT/BIN) was verified. Bitwise operations, the bit keyboard, data type switching and shift operations only had their entries confirmed in this round and were not exercised item by item, so they must not be presented as verified.

## Functions

### Use thousands / ten-thousands separator

Calculator supports thousands and ten-thousands separators and groups numbers by thousands by default (for example `12,345,678`).

- When the expression is in thousands, right-click the current expression area and select **Use ten-thousands separator**; the display is then grouped by ten thousands (for example `1234,5678`) and the history expressions change accordingly.
- When the expression is in ten thousands, right-click the current expression area and select **Use thousands separator** to restore thousands grouping.

The right-click menu also contains Undo, Redo, Cut, Copy, Paste, Delete and Select All. Both separator switches were actually verified.

> Note: this section describes the right-click separator switch verified in the Chinese edition. No English figure was captured for this scenario in this round, and no reused English figure is available for it.

### Symbolic Fault-tolerant Computing

Calculator supports keyboard operation and fault-tolerant computing of special symbols besides normal numbers and operation symbols. The input of expressions will not be affected by the input state or the case state of the keyboard.

- Fault-tolerance processing of multiplication: Input asterisk (*) or letter x to trigger multiplication; in this round * was verified to display as x.
- Fault-tolerance processing of division: Input division (/) to trigger division; in this round division was mainly entered with the button and the keyboard / mapping was not verified separately.
- Fault-tolerance processing of addition: Input addition (+) to trigger addition.
- Fault-tolerance processing of subtraction: Input minus (-) or underline (_) to trigger subtraction; in this round _ was verified to display as the minus sign.
- Fault-tolerance processing of percent sign: Input percent sign (%) to trigger the percent sign.
- Fault-tolerance processing of decimal point: Input an English decimal point (.) to trigger the decimal point normally. **The current 5.8.24.4 does not support entering a Chinese full stop to produce a decimal point; the character is simply ignored.** For example, entering `3。25` actually displays `325`, and entering `8。5*2_1` actually displays `85x2-1`. This differs from the description in the original system manual.
- Fault-tolerance processing of the bracket symbol: Input opening and closing brackets to trigger brackets.
- Fault-tolerance processing of equal sign: Input **=** or press the **Enter** key to trigger the equal sign; in this round Enter was verified to produce the result.
- Fault-tolerance processing of the clear symbol: Press **Esc** to trigger clearing; verified in this round.
- Fault-tolerance processing of the delete symbol: Press **Backspace** to trigger deleting; the delete button was available in this round but the keyboard Backspace mapping was not verified separately.
- Fault-tolerance processing of the letter symbol: Whether the keyboard is in upper or lower case, pressing the **A~F** keys triggers the activation of letters.

> Note: no English figure was captured for the symbol-tolerance scenario in this round, and no reused English figure is available for it; the verified Chinese figure is kept in the Chinese edition only.

### Expression

- Click = in the current expression input area or press the **Enter** key to perform the calculation and display the result in the current input box; the expression then becomes a historical expression.
- History: the history area lists the calculated expressions and their results in execution order. Results are grouped by thousands by default and can be used in further calculations.
- Re-edit: click a single historical expression to re-edit it. The expression is displayed in the expression input area. After editing, press the **Enter** key or = to modify the result of the historical expression and of any linked expression.
- Expression error: if the expression is incorrect, the calculation cannot be performed and a red "Expression error" message is displayed in the result area.

Verified in this round: typing `12345*6+7` displays `12,345x6+7`, pressing Enter gives `74,077`, and the history shows `12,345x6+7 = 74,077`; an extra closing bracket produces "Expression error".

> Note: no English figure was captured for the expression and history scenario in this round, and no reused English figure is available for it.

### Scientific Notation

In Standard Mode and Scientific Mode, when the calculation result is more than 16 digits or 32 digits respectively, it is displayed in scientific notation, that is, taking the first 16 digits / 32 digits multiplied by 10 to the power of plus or minus n.

- When the result is an integer and greater than 16 digits / 32 digits, it is displayed as: number + 15 digits / 31 digits after the decimal point + E + number.
- When the result is a decimal and greater than 16 digits / 32 digits, it is displayed as: number + 15 digits / 31 digits after the decimal point + E - number.

Click **F-E** to toggle scientific notation. Verified in this round: in Scientific Mode `2^200` with F-E enabled is displayed as `1.6069380442589902755419620923412E+60` and the F-E button is highlighted; in Standard Mode results longer than 16 digits are also converted to scientific notation (recorded as evidence in this round and not included as a formal figure).

![0|scientific_notation](fig/scientific_notation.png)

### Digital Linkage

- After an expression displays its numerical result, you can continue entering an operator; the first number of the new expression is then the result of the previous expression. Take the current expression 10 + 20 = 30 for example. After the result 30 is displayed, input + and then 9 to form a new expression 30 + 9; press the **Enter** key and the new result is 39.
- After two expressions are linked, modifying the numbers or operators of the previous expression affects the result of the linked expression if its own result changes. For example, given the linked expressions 10 + 20 = 30 and 30 + 9 = 39, changing the + operator of the first expression to x gives 10 x 20 = 200, and the second expression automatically becomes 200 + 9 = 209. According to this rule, up to 9 linked expressions are supported.
- When re-editing an expression that contains linked numbers, modifying a linked number or making the linked expression incorrect releases the linkage and cancels the number highlighting.

Verified in this round: `74,077+9 = 74,086` then `74,086+100 = 74,186`, forming multiple linked history entries with the linked numbers highlighted in blue.

> ![icon](../common/notes.svg)Notes: This function is only supported under Standard Mode.

## Main Menu

On the main menu you can switch the operation mode, switch the window theme, view the help manual, and get more information about Calculator. The main menu contains Mode, Theme, Help, About and Exit; all five entries were opened and confirmed.

> Note: no English figure was captured for the main menu in this round, and no reused English figure is available for it.

### Theme

The window theme includes Light Theme, Dark Theme and Follow System. In this round only the option names and the current selection (Follow System) were confirmed; the theme was not actually switched to verify the appearance change, so no verified conclusion is drawn about the visual result of switching.

1. On the Calculator interface, click ![main_menu](../common/icon_menu.svg).
2. Click **Theme** to select a theme.

### Help

View Help to get more information about Calculator. The menu item exists; the help window was not opened in this round and the opening behavior reuses the original system manual.

1. On the Calculator interface, click ![main_menu](../common/icon_menu.svg).
2. Select **Help**.
3. View the manual.

### About

1. On the Calculator interface, click ![main_menu](../common/icon_menu.svg).
2. Select **About**.
3. View the version description.

Verified in this round: the About dialog shows version **5.8.24.4**, homepage www.chinautos.com, the description and the acknowledgements.

> Note: no English figure was captured for the About dialog in this round, and no reused English figure is available for it.

### Exit

1. On the Calculator interface, click ![main_menu](../common/icon_menu.svg).
2. Click **Exit** to exit.

The menu item exists; the exit flow was not executed in this round and its behavior reuses the original system manual.
