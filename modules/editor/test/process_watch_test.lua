require 'lubyk'

app = mimas.Application()

pw = editor.ProcessWatch()

pw.list_view:show()

app:exec()