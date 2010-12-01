require('cv')

Outlet("mat_out", MatrixIO("Loaded image"))

-- Load image
--img = cv.LoadImage('~/Desktop/linux.jpg')
img = cv.LoadImage('../examples/GLLua/rk.png')
mat = cv.Mat(300, 300, cv.CV_8UC1)
cv.cvtColor(img, mat, cv.CV_RGB2GRAY)
-- rebuild texture
img_changed = true

mat_out:send(mat)

