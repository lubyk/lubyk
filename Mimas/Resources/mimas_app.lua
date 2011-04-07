require('lubyk')

app  = mimas.Application()

main = editor.Main()
main.library:sync()
main:setView(editor.MainView(main))
process_watch = editor.ProcessWatch(main)
main.main_view:show()
main.main_view:move(50, 50)
app:exec()