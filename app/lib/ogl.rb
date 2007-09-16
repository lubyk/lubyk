#!/usr/bin/env ruby -rubygems
# Name:  nehe_lesson02.rb
# Purpose: An implementation of NeHe's OpenGL Lesson #02
#          using ruby-opengl (http://nehe.gamedev.net/)
# 
# you need 'ruby-opengl' gem to run this

require 'rubygems'
require "gl"
require "glu"
require "glut"
require "mathn"

# Add GL and GLUT namespaces in to make porting easier
include Gl
include Glu
include Glut

module Rubyk
  class Matrix < Array
    def *(vector)
      res = []
      each do |rows|
        sum = 0
        rows.each_index do |i|
          sum += rows[i] * vector[i]
        end
        res << sum
      end
      res
    end
  
    def inspect
      s = ""
      each do |rows|
        l = "| "
        rows.each do |value|
          l += sprintf(' %4.2f',value)
        end
        s += l + " |\n"
      end
      s
    end
  end
end

class Ogl
  def initialize
    # object
    @rect  = [[-0.5,-0.5],
              [-0.5, 0.5],
              [ 0.5, 0.5],
              [ 0.5,-0.5]]
    # transformation matrix
    #trans = Matrix[[1.0,1.0], [0.0, 1.0]]
    phi   = 10 * Math::PI / 180.0
    trans = Rubyk::Matrix[  [Math.cos(phi), -Math.sin(phi)],
                     [Math.sin(phi),  Math.cos(phi)]]
    # new rect
    @rect2 = @rect.map do |v|
      trans * v
    end
    Thread.new do
      run
    end
  end
  
  # Placeholder for the window object
  def window
    ""
  end

  def draw_rect(r)
    glColor3f(1.0, 1.0, 1.0)
    glBegin(GL_QUADS)
      r.each do |p|
        glVertex3f(p[0],  p[1], 0.0)
      end
    glEnd
  end

  def draw_origin
    glPushMatrix
    glColor3f(1.0, 1.0, 0.0)
    glBegin(GL_LINES)
      glVertex3f(0,0,0)
      glVertex3f(0,1,0)
      glVertex3f(0,0,0)
      glVertex3f(1,0,0)
    glEnd
    glPopMatrix
  end

  def init_gl_window(width = 640, height = 480)
      # Background color to black
      glClearColor(0.0, 0.0, 0.0, 0)
      # Enables clearing of depth buffer
      glClearDepth(1.0)
      # Set type of depth test
      glDepthFunc(GL_LEQUAL)
    
      # Enable smooth color shading
      glShadeModel(GL_SMOOTH)

      glMatrixMode(GL_PROJECTION)
      glLoadIdentity
      # Calculate aspect ratio of the window
      gluPerspective(45.0, width / height, 0.1, 100.0)

      glMatrixMode(GL_MODELVIEW)

      draw_gl_scene
  end

  #reshape = Proc.new do |width, height|
  def reshape(width, height)
      height = 1 if height == 0

      # Reset current viewpoint and perspective transformation
      glViewport(0, 0, width, height)

      glMatrixMode(GL_PROJECTION)
      glLoadIdentity

      gluPerspective(45.0, width / height, 0.1, 100.0)
  end

  #draw_gl_scene = Proc.new do
  def draw_gl_scene
      # Clear the screen and depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

      # Reset the view
      glMatrixMode(GL_MODELVIEW)
      glLoadIdentity

      # Move left 1.5 units and into the screen 6.0 units
      glTranslatef(-1.5, 0.0, -6.0)

      # Draw a rectangle
      draw_rect(@rect)
      draw_origin
    
      # Move right 3 units
      glTranslatef(3.0, 0.0, 0.0)
    
      # Draw a rectangle
      draw_rect(@rect2)
      draw_origin

      # Swap buffers for display 
      glutSwapBuffers
  end

  # The idle function to handle 
  def idle
      glutPostRedisplay
  end

  def run
    # Keyboard handler to exit when ESC is typed
    keyboard = lambda do |key, x, y|
      case(key)
      when 27
        glutDestroyWindow(window)
        exit(0)
      end
      glutPostRedisplay
    end


    # Initliaze our GLUT code
    glutInit;
    # Setup a double buffer, RGBA color, alpha components and depth buffer
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("NeHe Lesson 02 - ruby-opengl version");
    glutDisplayFunc(method(:draw_gl_scene).to_proc);
    glutReshapeFunc(method(:reshape).to_proc);
    glutIdleFunc(method(:idle).to_proc);
    glutKeyboardFunc(keyboard);
    init_gl_window(640, 480)
    glutMainLoop();
  end
end