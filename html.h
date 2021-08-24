/*
  Webpage APP for ESP 8266 Ultrasonic Radar Car.
  Created by White Jiang , 2020.08.25
*/
#ifndef HTML_H
#define HTML_H
#include<Arduino.h>
const char html[] =R"(<!DOCTYPE html>
<html>
<head>
    <title>Smart Car Remote</title>
    <style>
        input {
            border-radius: 100px;
            height: 100px;
            width: 100px;
        }
        #main{
            width: 600px;
            display: grid;
            grid-template-columns: 50px 500px 50px;
            grid-template-rows: 80px 300px 80px 400px;
            grid-row-gap: 10px;
            grid-column-gap: 10px;
        }
        #canvas{
            grid-column-start: 1;
            grid-column-end: 4;
            width: 600px;
            height: 300px;
            border-style:solid;
            border-color: black;
            border-width: 1px;
        }
        * {
            margin:0 auto;
            padding: 0;
        }
        #rocker_bg {
            position: relative;
            width: 400px;
            height: 400px;
            background:white;
            border-radius: 50%;
            box-shadow: 0 0 5px 1px rgba(10,10,10,0.3);
        }
        #rocker {
            position: absolute;
            top: 31.25%;
            left:31.25%;
            width: 150px;
            height: 150px;
            border-radius: 50%;
            background: rgba(10, 10, 10, 0.4);
            z-index: 10;
            box-shadow: 0 0 5px 1px rgba(10,10,10,0.3);
        }
        #title{
            grid-column-start: 1;
            grid-column-end: 4;
        }
        #bar{
            grid-column-start: 1;
            grid-column-end: 4;
            display: grid;
            grid-template-columns:150px 150px 150px 150px ;
            box-shadow: 0 0 5px 1px rgba(10,10,10,0.3);
            margin: 0;
        }
        h1{
            font-size: 60px;
        }
        .slider{
            transform: rotate(-90deg);
            width: 400px;
            height: 50px;
            margin: 0;
            transform-origin: 200px 200px;
            box-shadow: 0 0 5px 1px rgba(10,10,10,0.3);
            
        }
        #rssi{
            display: inline-block;
            background: white;
            width: 75px;
            height: 75px;
            margin: auto auto;
            box-shadow: 0 0 4px 2px rgba(10,10,10,0.1);
        }
        #distance{
            display: inline-block;
            background: white;
            width: 75px;
            height: 75px;
            margin: auto auto;
            box-shadow: 0 0 4px 2px rgba(10,10,10,0.1);
        }
        #degree{
            display: inline-block;
            background: white;
            width: 75px;
            height: 75px;
            margin: auto auto;
            box-shadow: 0 0 4px 2px rgba(10,10,10,0.1);
        }
        #status{
            display: inline-block;
            background: white;
            width: 75px;
            height: 75px;
            margin: auto auto;
            box-shadow: 0 0 4px 2px rgba(10,10,10,0.1);
        }
        #power_div1{
            width: 100%;
            height: 100%;
            text-align: center;
        }
    </style>
    <script>
        function $(id) { return document.getElementById(id); }
        // var ws = new WebSocket("ws://192.168.3.106");
        var ws = new WebSocket("ws://" + window.location.host + "/");
        var connected=false;
        var json_input={};
        json_input.degree=0;
        json_input.distance=0;
        json_input.rssi=0;
        var powerValue=70;
        function scroll_set(){
            var psconsole = $('status');
            psconsole.scrollTop = psconsole.scrollHeight;
        }
        ws.onopen = function () {
            connected = true;
            $("status").innerHTML="Status: \n"+"Connected!";
            scroll_set();
        }
        ws.onmessage = function (evn) {
            json_input=JSON.parse(evn.data);
            scroll_set();
        }
        ws.onerror = function(){
            connected=false;
            $("status").innerHTML="Status: \n"+"Connection Failed!";
            scroll_set();
        }
        ws.onclose = function () {
            connected=false;
            $("status").innerHTML="Status: \n"+"Closed!";
            scroll_set();
        }
        function sendMassage(msg) {
            ws.send(msg);
            scroll_set();
        }
        function updateData(obj){
            $("rssi").innerText="rssi: \n"+obj.rssi;
            $("distance").innerText="distance: \n"+obj.distance;
            $("degree").innerText="degree: \n"+obj.degree;
        }
        var flag=0;
        function draw(radius,angle){
            let rad=Math.PI*(angle)/180;
            let x = 150+Math.cos(rad)*radius/400 * 150;
            let y = 150-Math.sin(rad)*radius/400 * 150;
            if (angle==135 || angle==45)
                flag=1;
            else
                flag=0;
            draw_circle(x,y);
        }
        function draw_circle(x,y){
            var canvas=$("canvas");
            if (canvas.getContext){
                
                var ctx = canvas.getContext('2d');
                if(flag)
                    ctx.clearRect(0,0,300,150);
                ctx.beginPath();
                ctx.arc(x, y, 1.5, 0, Math.PI*2, false);
                ctx.fill();
            }
        }
        function receive(){
            updateData(json_input);
            draw(json_input.distance,json_input.degree+45);
        }
        var val2 =setInterval(receive,20);
    </script>
</head>
<body onload="draw();">
    <div id="main">
        <div id="title">
            <h1>Smart Car</h1>
        </div>
        <canvas id="canvas"></canvas>
        <div id="bar">
            <div id="status"></div>
            <div id="rssi"></div>
            <div id="distance"></div>
            <div id ="degree"></div>
        </div>
        <div id="power_div1">
            <p id="powervalue">70%</p>
            <input class="slider" id="power" name="power" type="range" min="0" max="100" step="1" value="70" onchange="changePowerValue();"/>
        </div>
        <div id="rocker_bg">
            <div id="rocker"></div>
        </div>
    </div>
    <script>
        var series=256;
        // 摇杆容器大小、摇杆容器中心坐标、半径
        var rocker = document.getElementById('rocker_bg');
        var rocker_w=rocker.clientWidth;
        var rocker_h=rocker.clientHeight;
            // 起始坐标
        var start_X=rocker.offsetLeft;
        var start_Y=rocker.offsetTop;
        var rocker_center_X = start_X + rocker_w/2;
        var rocker_center_Y = start_Y + rocker_h/2;
        var R = rocker_w/2;
        // 摇杆、摇杆的大小
        var sphere = document.getElementById('rocker');
        var sphere_w=sphere.clientWidth;
        var sphere_h=sphere.clientHeight;
        // 摇杆起始坐标
        var sphere_X=(rocker_w - sphere_w)/2 ;
        var sphere_Y=(rocker_h - sphere_h)/2 ;
        // 摇杆与摇杆容器中心距离
        var distance=0.0;
        var pos = {};
        sphere.ontouchmove = function(e) { 
            e.preventDefault();
            // 获取当前坐标
            current_X = e.touches[0].clientX - start_X ;
            current_Y = e.touches[0].clientY - start_Y ;
            distance = Math.sqrt(Math.pow(current_X-rocker_w/2,2) + Math.pow(current_Y-rocker_h/2,2));
            
            // 限制边界
            if(distance>R){
                current_X = R/distance * (current_X - rocker_w/2) + rocker_w/2;
                current_Y = R/distance * (current_Y - rocker_h/2) + rocker_h/2;
            }
            // 让摇杆跟随手指
            sphere_X = current_X - sphere_w/2;
            sphere_Y = current_Y - sphere_h/2;
        }
        sphere.ontouchend = function(e) {
            // 让摇杆回到摇杆容器中心、让摇杆可以重新跟随
            distance=0.0;
            sphere_X=(rocker_w - sphere_w)/2 ;
            sphere_Y=(rocker_h - sphere_h)/2 ;
        } 
        function changePowerValue(){
            $("powervalue").innerText=$("power").value+"%";
            powerValue=$("power").value;
        }
        // 更新摇杆位置
        function send(){
            sphere.style.left = sphere_X + 'px';
            sphere.style.top = sphere_Y + 'px';
            pos.x=normalization(sphere_X + sphere_w/2)*(series-1);
            pos.y=normalization(sphere_Y + sphere_h/2)*(series-1);
            pos.power=Math.round((powerValue-1.0)/100.0*1023);
            sendMassage(JSON.stringify(pos));
            
        }
        function normalization(n){
            return (n-0.0)/(2.0*R);
        }
        var val1 =setInterval(send,20);
    </script>
</body>
</html>)";
#endif