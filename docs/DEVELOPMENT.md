Development process of the Accellera SystemC implementation
===========================================================
v1.0, June 2013

*An HTML-rendered copy of this document can be found at
 <https://github.com/OSCI-WG/systemc/blob/master/docs/DEVELOPMENT.md>.*


In this document, the internal development process for Accellera's SystemC
Language Working Group (and TLM Working Group) is described.  This mostly
relates to the development and maintenance procedures for the ASI
proof-of-concept implementation of SystemC (and TLM).  This document
focuses on the technical aspects related to the development of the ASI
SystemC implementation.  Legal and formal procedures are documented at
<http://accellera.org/about/policies>.


---------------------------------------------------------------------
Repository organization
---------------------------------------------------------------------

The central source code repository of the ASI SystemC implementation is
hosted in two [Git] [1] repositories at [GitHub](http://github.com).  The
repositories are private to the `OSCI-WG` organization and can be found at:

 * <https://github.com/OSCI-WG/systemc>             (core SystemC library)
 * <https://github.com/OSCI-WG/systemc-regressions> (regression test suite)

Members of the `OSCI-WG` organization with the necessary access rights can
clone the repositories via SSH from the locations

     git clone -o osci-wg git@github.com:OSCI-WG/systemc.git
     git clone -o osci-wg git@github.com:OSCI-WG/systemc-regressions.git

respectively.  To obtain access to the repositories and the GitHub
organization in general, LWG members can contact the LWG chairs at
<mailto:lwg-chair@lists.accellera.org> including their GitHub account name.

>  *Note:*
>  Using an explicit name of the 'remote' (`-o osci-wg`) is recommended
>  to allow using the default remote name `origin` for a personal fork
>  where you can push your changes by default, see [below](#basic-branch-setup).

Comprehensive documentation about [Git] [1], a distributed version control
system, can be found in the [Pro Git book] [2], also available online.
Since Git is 'distributed', it is a very natural choice for the distributed
development process needed for the collaboratively evolving proof-of-concept
implementation of SystemC.

In order to contribute changes to the different repositories, it is
recommended to create personal (or company-based) [forks] [3] of the
repositories on GitHub and push the proposed changes (bugfixes,
features, ...) there.  These forks are also only accessible to members
of the OSCI-WG organization. Details of the intended work-flow are
described in the next [section](#basic-branch-setup). It is convenient to
add this GitHub fork as a remote to your local clone of the
repository:

      cd <repo>/
      git remote add origin git@github.com:<your-account>/<repo>.git
      git branch --set-upstream master origin/master

Any changes can then be pushed to GitHub using:

      git push [options] [<repository>] [<refspec>...]

 * If you omit the `<repository>`, the default destination is
   the remote of the current branch (or `origin`).
 * The `<refspec>` basically follows the format
    `<local-branch>:<remote-branch>`, or just `<branch>`, if
   both are the same.
 * Omitting the `<refspec>` pushes all branches with 'matching'
   remote branches to the repository.

A basic cheat sheet containing the an overview of the general
Git commands and workflow can be found [online] [4].

[1]: http://git-scm.com "Git version control system"
[2]: http://git-scm.com/book "Pro Git Book"
[3]: https://help.github.com/articles/fork-a-repo
[4]: http://zrusin.blogspot.de/2007/09/git-cheat-sheet.html "Git Cheat Sheet"


---------------------------------------------------------------------
Development flow
---------------------------------------------------------------------


### Basic branch setup

The main idea is to have two main branches, one for the development,
one for the releases.

* **master**

  The latest and greatest `HEAD` of the LWG development.
  This is, were all the new features and fixes go.

* **release**

  This branch is used to create the release tarballs, both
  internal and public snapshots.

For the **regressions**, the `release` branch is more or less just a pointer
to the latest revision of a snapshot (or release).  It is still useful
to keep a named branching point, in case of required hotfixes.

For the **core library**, the `release` branch is to be different from the
`master` branch.  The idea is to fully track the contents of the released
tarball.  This requires the following changes compared to the `master`
branch:

  - The Automake generated files are added to this tree.
  - ASI internal files are stripped
    (`.gitignore`, internal documentation, ...).

To prepare a release, the `master` branch would then be merged into the
`release` branch, the automake files would be updated (if necessary) and
the clean working tree could be used as baseline for the tarball (e.g.,
via `git-archive(1)`).  Details are described in the next section
[release management](#release-management).  The history of the (core library)
repostitory could then look like shown in the following graph
(time progresses upwards):

       time  feature   master hotfix release
             branches    |         | |
        ^      |  |
        |                [master]
        |                |           [release]
        ^          ----- o           |
        |         /      |           o - [systemc-2.3.0.1]
        |        /    -- o          /|
        |       /    /   |         o |
        ^      |  o--   ...         \|
        |      o ...     |  -------- o - [systemc-2.3.0]
        |      |  o     .../         |
        |      o   \---- o -[public] ..
        ^       \       \|           |
        |        ------- o           o   (internal snapshot)
        |               ...          |
        ^                            o - [systemc-2.2.0]

It should usually be sufficient to keep the two branches `master`
and `release`, and cherry-pick hotfixes for emergency releases
directly on top of the `release` branch.  For convenience, an
additional `public` branch could be used to mark the branching
point for the last `release`.

If more sophisticated version branches are needed, a development
model similar to the well-known ["successful branching model"] [5]
can be deployed.  Not all aspects of this model are expected to
be needed for the ASI SystemC implementation, as we usually
maintain only a single (i.e., the latest) public release of the
kernel.

[5]: http://nvie.com/posts/a-successful-git-branching-model/ "'A successful Git branching model' by Vincent Driessen"


### Adding a feature (set)

The development of a new contribution in form of a feature or a
complex bug fix is best done in a new feature branch, which is
forked and checked out from the ASI `master` branch:

      git checkout -b <company>-<feature-xyz> master

Then code up the new contribution.  Please try to facilitate code
review by other ASI members by logically grouping your changes into
one commit per addressed issue. For the commit messages, please
consider to follow these suggestions: 

>  *Note:* **Commit messages**
>
>  Though not required, it’s a good idea to begin the commit message with
>  a single short (less than 50 character) line summarizing the change,
>  followed by a blank line and then a more thorough description. Tools
>  that turn commits into email, for example, use the first line on the
>  `Subject:` line and the rest of the commit in the body.

During the development of the contribution, the `master` branch may
receive other commits. In that case, consider rebasing the commits in
your feature branch onto the `HEAD` of the `master` branch to keep the
history clean. Once the contribution is ready for review by the
working group, push the feature branch in your fork of the respective
repository on GitHub:

      git push <your-github-fork-remote-name> <company>-<feature-xyz>

Then, send a [pull request] either manually or via [GitHub] [6] to
initiate the code review by the working group members.  The summary
can be manually generated by

      git request-pull master git@github.com/<account>/<repo>.git \
              <company-feature-xyz>

to be sent to the LWG reflector.

To review the proposed contributions, one can either browse the
repository at [GitHub] [6], or add the remote location to a local
clone of the repository

      # add the fork to your set of "remotes"
      git remote add <remote-name> git@github.com/<account>/<repo>.git
      git fetch  <remote-name>

      # examine differences
      git diff master..<remote-name>/<company-feature-xyz>
      git log <remote-name>/<company-feature-xyz>

After the contribution is accepted, it will be merged into the ASI
`master` branch by the responsible source code maintainer.  This should
be done with an explicit *merge commit*, to keep the individual 
contributions separated:

      git merge --no-ff --log \
         <remote-name>/<company-feature-xyz>

Instead of fully merging the contribution, the maintainer may choose
to cherry-pick individual commits or to rebase the feature branch on
an intermittently updated `master`. He may also request additional
changes to be done by the submitter. In that case, the submitter may
need to merge recent changes to the `master` branch into his feature
branch before carrying out the requested changes.

After the contribution has been fully merged into `master`, the
feature branch in the local and Github fork may be deleted.

      git branch -d <company-feature-xyz>      # delete local branch
      git push  origin :<company-feature-xyz>  # delete remote branch

[6]: https://help.github.com/articles/using-pull-requests "Using Pull Requests - github:help"


### Tracking release tarballs

In order to track the release tarballs (both LWG-internal and public
ones), a tool named [`pristine-tar`] [7] is used.  It enables tracking
archives directly within the repository.

When looking at the branches (e.g., by using `gitk --all`), you may notice
an additional, separate branch called `pristine-tar`.  This branch contains
the metadata for the pristine-tar tool.  `pristine-tar` enables
tracking original archives with minimal overhead, since only small binary 
deltas instead of the full tarballs are stored within the repository.

> *NOTE*: Currently, only Linux and Mac OS X are supported platforms for
>         `pristine-tar`.  Porting to Windows (MinGW) is to be done in the
>         future, which should be possible with reasonable effort to
>         support the required use cases within the ASI SystemC working
>         groups.


#### Basic workflow

      # adding an archive (done by the maintainer)
      pristine-tar [-m message] commit systemc-2.3.1.tgz release
      git push origin pristine-tar

      # retrieving an archive
      pristine-tar checkout systemc-2.3.0.tgz


The maintenance of the `pristine-tar` branch (i.e., adding new archives to
the `pristine-tar` branch) is done by the LWG chairs during the [release
management](#release-management).

[7]: http://joeyh.name/code/pristine-tar/ "pristine-tar homepage"


### Maintaining a private (set of) branches

*TODO*: Outline maintenance of an in-house forking model, seamlessly
        integrated with the LWG's flow.

Vendor's may be interested in maintaining their own, in-house flow
to align the internal development of a derived SystemC implementation,
while being able to pick fixes from the ASI tree (and hopefully)
contributing fixes and features back to the proof-of-concept
implementation.

For this purpose members may employ the already mentioned ["successful
branching model"] [4] by Vincent Driessen. The vendor can branch its
own development branch, e.g., `develop-<vendor>` from the already
tracked ASI's development branch `master` in his clone of the ASI
repository. The vendor is then able to integrate commits on the ASI
development branch by merging it into his his vendor development
branch.

Bug fixes to be contributed back to ASI consist usually of one or
several isolated commits. They need to be cherry picked from the
vendor's development branch into a new branch created from the ASI
development branch:

      git checkout -b <vendor>-fix-<bug> origin/master
      git cherry-pick <commit>...

Once, the bug fix branch is ready, it should be pushed into the
vendor's github account and a pull request created, as described in
the [feature branch section](#adding-a-feature-set).

A new feature consists usually of a series of commits developed in a
dedicated feature branched of the vendor's or ASI's development
branch. Only in the first case, a rebase on the top of the ASI's
development branch is necessary. To this end, branch first from the
feature branch:

      git checkout -b <vendor>-<new-feature> <private-feature-branch>
      git rebase [-i|--interactive] --onto origin/master develop-<vendor>

Once, the bug fix branch is ready, it should be pushed into the
vendor's github account and a pull request created, as described in
the [feature branch section](#adding-a-feature-set).


---------------------------------------------------------------------
Release management
---------------------------------------------------------------------

To prepare a new release tarball, the following set steps are to be
performed by the maintainer

0. **Prepare the release in the `master` branch**

  Before creating a release snapshot, the documentation and version
  information in the package should be updated within the `master`
  branch.  This includes files like
  - `ChangeLog`, `RELEASENOTES`, `README`, `INSTALL`
  - `src/sysc/kernel/sc_ver.h`,
    `src/tlm_core/tlm_version.h`

  During the release preparation phase, other functional changes
  should not be added/merged to the `master` branch.

1. **Update the `release` branch**

        # switch to release branch
        git checkout release

        # merge master branch
        git merge --no-commit master
        git rm <new-internal-file...> # drop new or changed "private" files
        git commit -m "merge master branch for x.x.x release"

2. **Update the Autoconf (and other auto-generated) files**

        autoreconf -if # or config/bootstrap
        git add -u     # add changed files
        git status     # check for untracked files
        git add <new files to distribute>
        git commit -m "update autogenerated files for x.x.x release"

3. **Tag the release revision**

  In order to keep track of the release snapshots, the revisions used
  for creating the release tarballs should be marked with an *annotated*
  and optionally signed Git tag.

        # git tag -a -m "<package> <version>" <package>-<version> <refspec>
        git tag -a -m "SystemC 2.3.0" systemc-2.3.0 release

  The tag name follow the pattern `<package>-<version>`, where `version`
  follows the versioning rules in IEEE 1666-2011, which means it should
  follow one of the three standard formats:
  * `x.x.x_beta_<isodate>` for beta/internal versions
  * `x.x.x_pub_rev_<isodate>` for public review versions, and
  * `x.x.x` for public release versions.

  *NOTE:* The tag should be on the `release` branch, to enable the
  automated tarball creation in the next step.

4. **Create the release tarball**

  `git archive` can then be used to create the release tarball.
  `git describe` can be used to obtain the correct tarball name
  based on the current tag.

        VERSION="`git describe release`" \
          git archive --format=tar.gz --prefix=${VERSION}/ release \
            > ${VERSION}.tar.gz

  *NOTE:* Even without a tag, a quick-shot release of the
          release branch can be generated this way.

  The resulting archive should then be added to the `pristine-tar`
  branch to keep track of the release history:

        pristine-tar commit ${VERSION}.tgz release

5. **Publish the release**

  Upload the archive to the LWG area for internal review and
  push the changes to GitHub.

        git push osci-wg \
               master release pristine-tar \
               <package>-<version>

  *NOTE:* The tag needs to be pushed explicitly.


---------------------------------------------------------------------
Issue tracking
---------------------------------------------------------------------

Open issues (bugs, cleanups, features) related to ASI's proof-of-concept
implementation of SystemC/TLM are tracked in GitHub's issue tracking system:

 * <https://github.com/OSCI-WG/systemc/issues>             (core library)
 * <https://github.com/OSCI-WG/systemc-regressions/issues> (regression tests)

Issues are grouped (by using labels) in the following categories for
different parts of the implementation:

 * `core`           - SystemC core language, i.e. everything in `sc_core`
 * `datatypes`      - SystemC datatypes, i.e. in `sc_dt`
 * `tlm`            - TLM-1.0, TLM-2.0
 * `infrastructure` - build system(s), scripts, etc.  

Additional labels are used to classify issues according to their
severity (10 highest), according to the following guidelines:

 * `10-critical`
 
   Show-stoppers that must be fixed, affects all (or at least most)
   platforms and violates fundamental specifications for most applications.

 * `09-serious`

   At least one of the explicitly supported platforms is affected and
   causes significant problems for many applications.
 
 * `06-medium`

   Covers an area, where the standard may not be clearly specified.  May
   require changes to external/standard API.
 
 * `05-feature`

   New feature proposal, beyond the current standard. Includes internal
   (and external, providing adoption by IEEE P1666 WG) API changes.
 
 * `04-errata`

   Inconvenience (errata) for users of many platforms, workaround available.
   Solution may require internal API changes.

 * `02-documentation`

   Documentation inconsistency or insufficiency (e.g. whitepaper unclear
   or misleading), no code changes.

 * `01-inconvenience`

   Inconvenience (workaround available), for some platforms
   (e.g. users of Visual Studio 2003)
 
 * `00-cosmetic`

   Changes addressing performance or clarity of implementation,
   no API changes. 

The discussion on issues usually starts on the LWG reflector or during the
LWG meetings.  After an initial consensus on the "validity" of the issue,
the issue is added to the issue tracking system, a classification is done
(including a target milestone), and preferably a responsible person is
assigned.


---------------------------------------------------------------------
Changelog
---------------------------------------------------------------------

* v1.0 - Development flow implemented at GitHub (2013-06-20)

 * Issue tracking documented

* v0.9 - Initial proposal to the ASI LWG (2012-09-25)

 * To be discussed during the October LWG phone conference


---------------------------------------------------------------------
Authors
---------------------------------------------------------------------

  * Philipp A. Hartmann <philipp.hartmann@offis.de>
  * Torsten Maehne      <Torsten.Maehne@lip6.fr>

