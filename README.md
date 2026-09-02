# Ark_Race2026
<img width="388" height="291" alt="image" src="https://github.com/user-attachments/assets/e4b470f6-d2f5-4689-8a26-201f5c9b24f3" />

2026第五届方舟计划机器人大赛自述留档

## 废话
虽然这东西像破烂,但确实是我想出来的,但只停在想象,没考虑实际就直接做了,吸取教训吧🤫

## 目录结构
- 1.Embedded Team 电控组文件
- 2.Mechanical Group 机械组文件
- 3.培训‑机械组培训内容
- 4.Embedded Team\OLEDtest 整车电控文件，推荐 VSCode 打开
    - 其余为旧版本、临时文件、商家文件
- 5.Mechanical Group\我说元神牛逼你说队‑机械建模文件
    - 其余为26场地文件，参赛手册
      
## 📌 整车基础参数
- 参赛赛事：第五届方舟计划机器人大赛
- 整车重量：5.7kg（含遥控器）

---

## ⚙️ 一、硬件选型问题复盘
### 1. 驱动电机
- 核心问题：未计算整车负载扭矩，电机扭矩余量严重不足
- 实际表现：整车负重后速度疲软、动力不足
- 结构不匹配：该电机轴径小、原生不适配大直径车轮，重载工况非常吃力
- 迭代方案：后续可更换 24V 520 / 550 大扭矩电机，适配重载大轮场景
<img width="342" height="151" alt="image" src="https://github.com/user-attachments/assets/abe53f5e-4c99-4494-ae84-3a686c66b4d5" />

### 2. 法兰联轴器
- 现状：电机轴仅4mm，市面仅能匹配小型单面法兰联轴器
- 致命缺陷：大麦轮自重+惯性大，仅靠单颗六角螺母锁止完全不够
- 实战问题：全向平移时车轮微震、螺丝松动、运行极易掉轮
<img width="202" height="176" alt="image" src="https://github.com/user-attachments/assets/3cedb741-f5cd-42b0-87f7-148729f2d25b" />


### 3. 主控板子（嘉立创天空星 F407VET6）
- 优点：F407芯片资源充足、主频、外设、内存完全满足比赛需求
- 事故问题：调试期间两次开发板烧毁
- 疑似原因：带电插拔、不断电操作导致瞬间电压冲击击穿
- 规范整改：后续所有接线、插拔、改装必须全程断电操作
<img width="554" height="130" alt="image" src="https://github.com/user-attachments/assets/5275b7fd-f8e8-41ee-803c-40e6027c8969" />

---

## 🏗️ 二、底盘结构设计缺陷
### 1. 平板式底盘
- 结构短板：纯平面薄板底盘，不适合大尺寸、大重量车体
- 实际问题：整车5.7kg负重后底盘下沉形变，车轮倾角偏移
- 核心痛点：麦轮全向解算极度依赖结构刚性，形变误差软件完全无法补偿
- 经验总结：平板底盘仅适合小型轻量化小车，应该考虑采用铝型材框架的
<img width="448" height="240" alt="image" src="https://github.com/user-attachments/assets/d2f461b4-ce86-4eab-b050-d4a904efe003" />

### 附件可参考设计
<img width="271" height="214" alt="image" src="https://github.com/user-attachments/assets/07f1d5b9-a204-4670-9f92-06b5fc28cd52" />
<img width="319" height="212" alt="image" src="https://github.com/user-attachments/assets/027aa8c9-495c-43aa-aa98-224ed409633f" />

---

## 📐 三、升降传送机构重大设计问题
### 1. 单边受力倾覆问题
- 结构硬伤：升降横梁左侧承载负载，产生巨大下压倾覆力矩
- 受力畸形：动力源集中在最右侧，受力点严重不对称
- 最后会出现光轴滑块卡顿、升降不顺、结构偏斜
<img width="386" height="233" alt="image" src="https://github.com/user-attachments/assets/e257cde0-3c5b-4ac6-8b9c-a276e9943c5b" />

### 2. 竖直立柱无闭环固定
- 设计漏洞：两根500mm高竖杆顶部无固定框架，未形成刚性矩形闭环
- 运动缺陷：车体一动，立柱整体高频抖动、振幅极大
(设计的时候以为横梁可以稳定这两根杆)

### 3. 步进电机固定方案简陋
- 现状：前期未预留标准安装孔位
- 临时方案：仅靠胶水+扎带固定，刚性极差、极易松动
- 改进：后续结构建模优先预留电机固定座、孔位
  
### 迭代优化思路
- 将传送驱动轴居中布置，右侧改为纯支撑光轴，弱化单边倾覆力
- 单根导向杆改为采用双滑块/四滑块到龙门架的结构，大幅提升承载稳定性
<img width="408" height="197" alt="image" src="https://github.com/user-attachments/assets/262ab4a2-e309-4d43-96bc-34c25419f43c" />

### 附件可参考设计
<img width="178" height="127" alt="image" src="https://github.com/user-attachments/assets/fc0cc52e-49ef-4b24-a199-ab001d493d93" />
<img width="132" height="152" alt="image" src="https://github.com/user-attachments/assets/b6e811ba-addb-411c-bac9-15955f839b2d" />


---

## 🤌 四、末端夹取机构总结
- 现状：舵机夹爪为赛前一晚才买的配件
- 优点：常规抓取表现尚可，适配比赛基础需求
- 缺陷：夹爪行程不足，无法包覆半个球体，抓取容错率低
- 后续升级方向：气动夹爪 / 磁吸抓取结构，稳定性、负载能力全面升级
  <img width="234" height="209" alt="image" src="https://github.com/user-attachments/assets/02378867-5fc8-4771-80cd-41b1eee0cf39" />

### 附件
<img width="553" height="737" alt="image" src="https://github.com/user-attachments/assets/a0b469c4-8801-4e65-835c-a1c9122b04bf" />
<img width="553" height="415" alt="image" src="https://github.com/user-attachments/assets/6c77cd5f-1547-49a7-b425-ca388f2ce71c" />
<img width="2628" height="1848" alt="1f232b22a2a29387cc7f6337f348d309" src="https://github.com/user-attachments/assets/4215e590-452e-4e3e-a917-90cfb44487a3" />


---

