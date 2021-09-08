# TODO
- 为大作业pre做一套专门的剧情和地图
- 写注释

# 战棋大作业
具体而言，本次大作业部分地实现了一套以 json 为存储格式的战棋编辑器、完整地实现了其执行框架。由于时间、精力等有限，编辑器部分只完成了地图编辑器部分。下面结合项目对 json 部分做出说明。

首先，读取 game\game.json。此处本可以使用对话框选择文件读取，但出于展示目的改成了硬编码的读取路径。一个典型的 game.json 结构如下：
```
{
	"ally": {
		"alice": {
            "atk": 7,
            "def": 6,
            "hp": 18,
			"mov": 6,
            "range": 1
        },
        "micheal": {
            "atk": 7,
            "def": 6,
            "hp": 18,
			"mov": 6,
            "range": 1
        }
	},
	"battle": [
		{
			"before": "bdial01.json",
			"map": "test.json"
		},
		{
			"map": "test.json",
			"after": "adial02.json"
		}
	]
}
```
其中描述的文件路径为相对于 game.json。

ally 对象描述了若干个友方角色，其只包含初始属性，具体的角色贴图由地图文件指定。battle 数组描述了若干场战斗，每场战斗由战前对话、战斗和战后对话组成，在战斗间可以按照角色的升级进行加点。角色的属性包括攻击力、防御力、机动性、生命值、攻击距离、等级和经验，攻击是单方向的，造成 max(攻击方atk-受攻击方def, 攻击方atk*0.1向上取整) 的伤害，根据是否破防播放不同音效。

一个典型的地图文件 json 如下：
```
{
    "bgm": "The Blue Reverberation.mp3",
    "name": "test",
    "width": 50,
    "height": 50,
    "tile": {
        "fog": {
            "bimage": "b_road.png",
            "fimage": "f_fog.png",
            "cost": 2,
            "damage": 1,
            "pass": true,
            "pos": [
                {
                    "x": 2,
                    "y": 1
                },
                {
                    "x": 2,
                    "y": 2
                },
                {
                    "x": 2,
                    "y": 3
                },
                {
                    "x": 2,
                    "y": 4
                }
            ]
        },
        "grass": {
            "bimage": "b_road.png",
            "fimage": "f_grass.png",
            "cost": 2,
            "damage": 0,
            "pass": true,
            "pos": [
                {
                    "x": 0,
                    "y": 0
                },
                {
                    "x": 0,
                    "y": 2
                },
                {
                    "x": 0,
                    "y": 3
                },
                {
                    "x": 1,
                    "y": 2
                },
                {
                    "x": 1,
                    "y": 3
                }
            ]
        },
        "null": {
            "bimage": "null.png",
            "fimage": "",
            "cost": 1,
            "damage": 0,
            "pass": false,
            "pos": [
            ]
        },
        "river": {
            "bimage": "b_river.png",
            "fimage": "",
            "pass": false,
            "pos": [
            ]
        },
        "road": {
            "bimage": "b_road.png",
            "fimage": "",
            "cost": 1,
            "damage": 0,
            "pass": true,
            "pos": [
                {
                    "x": 0,
                    "y": 1
                },
                {
                    "x": 1,
                    "y": 1
                },
                {
                    "x": 4,
                    "y": 1
                },
                {
                    "x": 4,
                    "y": 2
                },
                {
                    "x": 4,
                    "y": 3
                },
                {
                    "x": 5,
                    "y": 1
                },
                {
                    "x": 5,
                    "y": 2
                },
                {
                    "x": 5,
                    "y": 3
                },
                {
                    "x": 6,
                    "y": 1
                },
                {
                    "x": 6,
                    "y": 2
                },
                {
                    "x": 6,
                    "y": 3
                }
            ]
        }
    },
    "default": "null",
    "ally": {
        "alice": {
            "image": "alice.png",
            "pos": [
                {
                    "x": 4,
                    "y": 1
                }
            ]
        },
        "micheal": {
            "image": "micheal.png",
            "pos": [
                {
                    "x": 0,
                    "y": 1
                },
                {
                    "x": 0,
                    "y": 2
                },
                {
                    "x": 5,
                    "y": 1
                }
            ]
        }
    },
    "enemy": {
        "catdevil": {
            "image": "catdevil.png",
            "atk": 5,
            "def": 3,
            "hp": 10,
            "mov": 5,
            "range": 1,
            “exp”: 10,
            "pos": [
                {
                    "x": 4,
                    "y": 3
                },
                {
                    "x": 6,
                    "y": 3
                }
            ]
        },
        "catdragon": {
            "image": "catdragon.png",
            "atk": 5,
            "def": 3,
            "hp": 10,
            "mov": 5,
            "range": 2,
            "exp": 25,
            "pos": [
                {
                    "x": 1,
                    "y": 2
                },
                {
                    "x": 5,
                    "y": 3
                }
            ]
        }
    },
    "lose": [
        "micheal"
    ],
    "win": [
        "catdragon",
        "catdevil"
    ]
}
```
其中描述的文件路径为相对于该 json。

bgm 指明了该战斗的背景音乐；name 对象指明了该地图的名称，该名称会在需要时展示；width 和 height 指定了地图的尺寸。tile 对象囊括了一系列战棋地图的基本单元及各自位置，default 对象则说明了当某个位置未指明 tile 时的默认 tile。enemy 和 ally 对象分别指明了该地图下的所有角色，ally 对象没有详细的属性描述，因为该对象的属性会随着等级和加点发生改变，基础属性已在 game.json 中描述。lose 和 win 数组则指明了胜负条件，当其所含的对象均死亡则判定胜负。即使是同一个友方角色，也可能存在多个实例，这本是为了实现某些技能，但由于时间、精力等原因，技能被砍掉了。

对 tile 对象的详细说明：每个 tile 有一个前景和一个背景，当该格内有角色时，角色会显示在前景和背景之间，同时这种划分也提高了资源的复用性。pass 属性说明了该 tile 是否可通行，若可通行才会有 cost 和 damage 属性。角色最初拥有相当于其 mov 的行动力，每经过一个 tile，则消耗其 cost 的行动力，当行动力小于等于 0 时就不可以再移动了。在回合开始时位于该 tile 的角色受到相当于其 damage 的伤害，damage 为负则为恢复生命。

配套的地图编辑器可以方便地修改地图。

# 代码要求
C++ 与 Qt 编写 √

面向对象的编程思想 √

统一的代码风格 √

包含必要的注释 √

合理设计类之间的关系、保证良好封装 partly

较高的运行效率 √

# 功能要求
## 界面、交互设计
标题界面和游戏界面 √

可查看角色和敌人的详细信息 √

通过鼠标实现角色控制 √

提高要求：
- 基于 json 读取的、完整的战斗前后剧情 UI
- 战斗间的升级加点 UI

## 地形设计
网格状地图 √

阻挡型地形 √

提高要求：
- 损伤型地形，在回合开始时处于该格中的角色会受到伤害
- 妨碍型地形
- 地形分为前景和背景，角色会显示在前景和背景之间
- 基于 json 读取生成的 tile，修改 json 可以方便地创造新地形

## 敌我方角色设计
基本属性：生命值、攻击力、机动性、攻击范围 √

基于初始属性和加点的、由玩家自行组合打造的不同特点可操控角色，如生命值高、机动性强、攻击范围大等 √

提高要求：
- 其他属性：防御力
- 经验、升级与加点
- 敌方 boss 型人物设计
- 基于 json 读取的敌我属性，可以自由地设置敌人属性等，boss 即为调整属性生成

## 战斗设计
回合制机制，每回合每个角⾊可以各移动和攻击⼀次，若不移动直接攻击也同样结束该角色行动 √

在地图上可视化可移动的范围（绿）、可移动后攻击到的敌方角色（黄）和可直接攻击到的敌方角色（红） √

基本的⾏动顺序，即⼀⽅所有角⾊⾏动结束后轮到另⼀⽅⾏动 √

战⽃伤害结算 √

## 关卡设计
⾄少设计两个关卡，有不同的地形，对我⽅可放置角⾊的种类和数量有不同限制 √

基本关卡通关判定和失败判定，即敌⽅全灭或我⽅全灭 √

提高要求：
- 其他通关/失败判定方式：某个/某些角色阵亡，实质是把消灭所有敌军/所有友方阵亡作为判定的特例
- 所有关卡均由 json 读取生成
- 附带地图编辑器，可以可视化地编辑地形、编辑敌我角色位置

## 敌⽅AI设计
基于 a-star 的寻路算法，基本的⾏动逻辑，即会不断靠近我⽅角⾊并发起攻击 √

## 其他要求
背景音乐、音效 √

图片素材 √

动画效果 partly 当受到伤害时有伤害数字显示

# 程序说明
## 逻辑
MainWindow 类负责 UI 呈现，GameController 类负责游戏流程控制，Dialog 类负责呈现幕间对话的图片及文本；Tile 类、Character 类分别负责存储瓦片和玩家信息，QGraphicsTileItem 类负责实现一个具体的 Tile 功能及视觉效果以及一切地图交互，Map 类负责实例化 QGraphicsTileItem。

## 流程
GameController 类有三个函数负责整个流程控制，分别是 drawBeforeDialog()、drawNextBattle()、drawAfterDialog()。开始游戏时，MainWindow 创建好 GameController 后，调用其 drawBeforeDialog() 函数，这个函数会加载战斗前对话，对话更新交由 Dialog 实例完成，并在对话全部播放完后由 Dialog 发出信号、加载加点界面；加点完成后进入战斗，加载战斗地图，战斗地图的所有行为均由 QGraphicsTileItem 类负责完成，Map 类只有辅助作用，实际运行跟 Map 类没有关系；当点击 QGraphicsTileItem 类的实例时发出信号由 MainWindow 负责 UI 的呈现，并根据一系列判断进行如寻路、移动、攻击等行动，由 Character 类进行伤害结算和音效播放，由 GameController 进行击杀结算并判断胜利或失败，判定后交由 MainWindow 类进行相应处理。

## 地图编辑器
它可以加载已有的地图文件，根据已有的 Tile 和 Character 编辑其位置。本来该有添加 Tile 和 Character、设置 width 和 height 的功能，但由于工期紧张和不加额外的分数就砍掉了。

## 更具体的说明
### Character
ally 属性指明是友方还是敌方。对于友方，有作为 meta 的友方和在战斗中实例化出的友方，战斗中的友方为 meta 友方实例化的对象，这是为了支持曾经想做的一些技能，但由于时间、精力不足等原因。技能被砍掉了；为了进行经验结算，战斗时的友方会保存其源信息，交由其进行经验的获取，并在幕间加点。

### QGraphicsTileItem
这玩意真长，我不该给它叫这个名。它是 QGraphicsItemGroup 的派生类，有四个 QGraphicsPixmapItem 成员，分别负责背景、前景、角色和叠加层的显示。角色会显示在背景和前景之间，这在地图绘制时显得比较真实。叠加层用于显示可移动、可攻击、在范围内等不同颜色，在点击友方角色时会显示出来。

## 演示
要啥自行车。

## 参考⽂献或引⽤代码出处
只查阅了 [Qt 的文档](https://doc.qt.io)，一切代码都是我自己写的，没有参考或引用任何其他项目或互联网已有代码。[本项目](https://github.com/HeartyYF/warchess)和[编辑器](https://github.com/HeartyYF/warchess-mapeditor)均在 GitHub 上开源，可以完整地追溯一切更改记录。

# 借物表
人物形象绘制：
- 小时

图片素材：
- 太阳照常升起 电影原声大碟 专辑封面

音乐及音效：
- Abandoned_Atk - Library of Ruina
- Defense_Evasion - Library of Ruina
- 2_1_Lobby - Library of Ruina
- 7_4_The Blue Reverberation - Library of Ruina
- 太阳照常升起 - 久石譲