/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pill.ts                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 18:12:26 by tde-cama          #+#    #+#             */
/*   Updated: 2022/04/13 21:01:47 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

import P5 from 'p5'
import { Paddle } from './paddle'
export class Pill{

    x: number
    y: number
    width: number
    xSpeed: number
    ySpeed: number
    radius: number
    winHeight: number
    xPosition: number
    yPosition: number
    isVisible: boolean

    constructor(p5, player1: Paddle){
        this.x = p5.windowWidth / 2
        this.y = p5.windowHeight / 2
        this.width = player1.height * 10/100
        this.radius = this.width / 2
        this.winHeight = p5.windowHeight
        this. isVisible = false
        this.start(p5)
    }

    update = (p5, player1: Paddle, player2: Paddle,) => {
        this.width = player1.height * 15/100
        if(this.y + this.radius >= p5.windowHeight || this.y - this.radius <= 0){
            this.ySpeed *= -1
        }
        if(this.x - this.radius >= 0 && this.x + this.radius < p5.windowWidth)
            this.move()
        else
            this.isVisible = false
        if(this.hitPlayer1(player1))
            this.shrink(player1)
        else if(this.hitPlayer2(player2))
            this.shrink(player2) 
        this.xPosition = this.x / p5.windowWidth
        this.yPosition = this.y / p5.windowHeight
    }

    move = () => {
        this.x += this.xSpeed
        this.y += this.ySpeed
    }

    start = (p5) => {
        const m: number[] = [-1, 1]
        this.xSpeed = Math.floor(Math.random() * (p5.windowWidth * 1/100)) + (p5.windowWidth * 0.5/100)
        this.ySpeed = Math.floor(Math.random() * (p5.windowWidth * 1/100)) + (p5.windowHeight * 0.15/100)
        this.xSpeed *= m[Math.floor(Math.random() * 2)]
        this.ySpeed *= m[Math.floor(Math.random() * 2)]
    }

    shrink = (player: Paddle) => {
        const originalHeight = player.heightProp
        player.heightProp = 10/100
        setTimeout(() => player.heightProp = originalHeight, 5000)
        this.isVisible = false
    }

    hitPlayer1 = (player1: Paddle): boolean => {
        if(this.x >= player1.x){
            if(this.x - this.radius <= player1.x + player1.width && this.onPaddle(player1))
               return true 
        }
        return false
    }
    hitPlayer2 = (player2: Paddle): boolean => {
        if(this.x <= player2.x + player2.width){
            if(this.x + this.radius >= player2.x && this.y >= player2.y && this.onPaddle(player2))
               return true 
        }
        return false
    }
    onPaddle = (player: Paddle): boolean => {
        if(this.y >= player.y && this.y <= player.y + player.height) 
            return true
        return false
    }
    
};