#puts "Hello Everybody.."































class RubykServer
  def runner
    puts Time.now.strftime(" %H:%M:%S\e[11D")
    sleep 1
  end
end
























#
@ogl ||= Ogl.new
#
class << @ogl
  def draw_gl_scene
      phi = (Time.now.to_f % 60) * 360 / 5
      phi = phi * Math::PI / 180.0
      x = Math::cos(phi) * 0.5
      y = Math::sin(phi) * 1.5
      # Clear the screen and depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

      # Reset the view
      glMatrixMode(GL_MODELVIEW)
      glLoadIdentity

      # Move left 1.5 units and into the screen 6.0 units
      glTranslatef(-x, -y, -6.0)

      # Draw a rectangle
      draw_rect(@rect)
      draw_origin
    
      # Move right 3 units
      glTranslatef(2*x, 2*y, 0.0)
    
      # Draw a rectangle
      draw_rect(@rect2)
      draw_origin

      # Swap buffers for display 
      glutSwapBuffers
  end
  
  def reload_gl
    glutDisplayFunc(method(:draw_gl_scene).to_proc);
  end
end

@ogl.reload_gl