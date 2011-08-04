require('lubyk')

app  = mimas.Application()

main = editor.Main()
main.splash_view = editor.SplashScreen(main)
main.splash_view:show()
app:exec()