_out_([=[<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>]=])
_out_(self.sections[1].title)
_out_([=[</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">



    <link href="css/bootstrap.css" rel="stylesheet">
    <link href="css/bootstrap-responsive.css" rel="stylesheet">
    <link href="css/docs.css" rel="stylesheet">
  </head>

  <body>
    <div class="header">
      <div class="container">
        <div class='row'>
          <div class='span12'>
            ]=])
_out_(self.header)
_out_([=[

          </div>
        </div>
      </div>
    </div>

    <div class="container">

      <div class="row">

        <div class='span3'>
          <ul class='nav nav-tabs nav-stacked local-nav affix'>
            ]=])
 for i, section in ipairs(self.sections) do 
_out_([=[
            <li><a href='#]=])
_out_(section.name)
_out_([=['><i class='icon-chevron-right'></i> ]=])
_out_(i == 1 and self.fullname or section.title)
_out_([=[</a></li>
            ]=])
 end 
_out_([=[
          </ul>
        </div>

        <div class='main span7'>

          ]=])
 for _, section in ipairs(self.sections) do 
_out_([=[
          <section id=']=])
_out_(section.name)
_out_([=['>
          <h2 class='section'>]=])
_out_(section.title)
_out_([=[</h2>

          ]=])
 for _, group in ipairs(section) do 
_out_([=[
          ]=])
 if group.class then 
_out_([=[
          <a name=']=])
_out_(string.gsub(group.class, '.', '-'))
_out_([=['></a>
          <h4 class='klass'><a href=']=])
_out_(group.class)
_out_([=[.html'>]=])
_out_(group.name)
_out_([=[</a></h4>
          ]=])
 elseif group.fun then 
_out_([=[
          <a name=']=])
_out_(group.fun)
_out_([=['></a>
          <h4 class='method'><span>:</span>]=])
_out_(group.fun)
_out_([=[ <code>]=])
_out_(group.params)
_out_([=[</code></h4>
          ]=])
 elseif group.class_fun then 
_out_([=[
          <a name=']=])
_out_(group.class_fun)
_out_([=['></a>
          <h4 class='function'><span>.</span>]=])
_out_(group.class_fun)
_out_([=[ <code>]=])
_out_(group.params)
_out_([=[</code></h4>
          ]=])
 elseif group.global_fun then 
_out_([=[
          <a name=']=])
_out_(group.class_fun)
_out_([=['></a>
          <h4 class='function'>]=])
_out_(group.global_fun)
_out_([=[ <code>]=])
_out_(group.params)
_out_([=[</code></h4>
          ]=])
 end 
_out_([=[

          ]=])
 for _, para in ipairs(group) do 
_out_([=[
          ]=])
_out_(private.paraToHtml(self, para))
_out_([=[

          ]=])
 end 
_out_([=[
          ]=])
 end 
_out_([=[
          </section>
          ]=])
 end 
_out_([=[
        </div>

        <div class='span2'>
          <ul class='nav nav-tabs nav-stacked'>
            ]=])
 if self.module then 
_out_([=[
            <li class='module'><a href=']=])
_out_(self.module)
_out_([=[.html'>]=])
_out_(self.module)
_out_([=[</a></li>
            ]=])
 end 
_out_([=[
            ]=])
 for _, name in ipairs(self.navigation) do 
              local fullname = self.navigation[name].__fullname
            
_out_([=[
            <li]=])
_out_( fullname == self.fullname and " class='active'" or "" )
_out_([=[><a href=']=])
_out_(fullname)
_out_([=[.html'>]=])
_out_(name)
_out_([=[</a></li>
            ]=])
 end 
_out_([=[
          </ul>
        </div>
      </div>
    </div>

    <footer class="footer">
    <div class="container">
      ]=])
_out_(self.footer)
_out_([=[

    </div>
    </footer>

    <script src='https://google-code-prettify.googlecode.com/svn/loader/run_prettify.js?lang=lua&skin=default'></script>
    <script type="text/x-mathjax-config">
      MathJax.Hub.Config({]=])
_out_([=[

        config: ["MMLorHTML.js"],
        jax: ["input/TeX","input/MathML","output/HTML-CSS","output/NativeMML"],
        extensions: ["tex2jax.js","mml2jax.js","MathMenu.js","MathZoom.js"],
        TeX: {]=])
_out_([=[

          extensions: ["AMSmath.js","AMSsymbols.js","noErrors.js","noUndefined.js"]
        }]=])
_out_([=[,
        menuSettings: {]=])
_out_([=[

          zoom: "Hover",
          mpContext: true,
          mpMouse: true
        }]=])
_out_([=[,
      }]=])
_out_([=[);
    </script>
    <script type="text/javascript"
      src='https://c328740.ssl.cf1.rackcdn.com/mathjax/latest/MathJax.js'></script>
    <script src='http://code.jquery.com/jquery.js'></script>
    <script src='js/bootstrap.min.js'></script>
  </body>
</html>

]=])

==== lk.Doc                       ( 1 test  : skipped 5): 1 Failure(s)
  5. Should convert to html
     [string "_out_([=[<!DOCTYPE html>..."]:149: bad argument #1 to 'ipairs' (table expected, got nil)
     stack traceback:
     	[C]: in function 'ipairs'
     	[string "_out_([=[<!DOCTYPE html>..."]:149: in function 'func'
     	...s/gaspard/git/lubyk/modules/dub/lib/dub/Template.lua:102: in function <...s/gaspard/git/lubyk/modules/dub/lib/dub/Template.lua:92>
     	(tail call): ?
     	(tail call): ?
     	test/Doc_test.lua:68: in function <test/Doc_test.lua:66>
     	(tail call): ?



Only testing 'convertToHtml'.
Fail... 1 failure / 1 test

